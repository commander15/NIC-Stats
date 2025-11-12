#include "test.h"

#include <common/error.h>
#include <statistics/statisticswritter.h>
#include <enveloppe/enveloppereader.h>
#include <enveloppe/enveloppewriter.h>

#include <QDir>

Package readPackage(const QString &dir)
{
    Package package;
    package.setDate(QDate::currentDate());

    EnveloppeReader reader;
    Error readerError;

    QDir directory(dir);
    directory.refresh();

    const QStringList files = directory.entryList({ "*.xlsx" }, QDir::Files, QDir::Name);
    for (const QString &file : files) {
        const Enveloppe enveloppe = reader.readEnveloppe(directory.filePath(file), &readerError);
        if (readerError.code() == 0)
            package.addEnveloppe(enveloppe);
        else
            QTextStream(stderr) << readerError.errorString() << Qt::endl;
    }

    return package;
}

int test(const QString &inputDir, const QString &outputDir, AbstractKitManager *kitManager) {
    const Package package = readPackage(inputDir);
    const QList<Enveloppe> enveloppes = package.enveloppes();

    EnveloppeWriter enveloppeWriter(kitManager);
    Error writerError;

    for (const Enveloppe &enveloppe : enveloppes) {
        enveloppeWriter.writeEnveloppe(enveloppe, outputDir + '/' + enveloppe.number() + ".xlsx", &writerError);
    }

    // Computing stats
    StatisticsCalculator calculator(kitManager);
    const Statistics stats = calculator.compute(package, StatisticsCalculator::NoOptions);

    // Writing stats to file
    StatisticsWritter statsWritter(kitManager);
    Error statsError;
    statsWritter.writeStatistics(stats, outputDir + "/STATISTIQUES.xlsx", &statsError);

    // Checking errors

    if (writerError.code() != 0) {
        QTextStream(stderr) << "WRITE: " << writerError.errorString() << Qt::endl;
        return 1;
    }

    return 0;
}
