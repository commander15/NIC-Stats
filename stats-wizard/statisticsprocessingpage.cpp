#include "statisticsprocessingpage.h"
#include "ui_statisticsprocessingpage.h"

#include <enveloppe/enveloppereader.h>
#include <enveloppe/enveloppewriter.h>
#include <statistics/statisticswritter.h>

#include <QAbstractButton>
#include <QThread>
#include <QDir>

StatisticsProcessingPage::StatisticsProcessingPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::StatisticsProcessingPage)
{
    ui->setupUi(this);

    m_timer.setInterval(3000);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, [this] {
        emit completeChanged();
        QTimer::singleShot(100, wizard(), &QWizard::next);
    });

    setCommitPage(true);
}

StatisticsProcessingPage::~StatisticsProcessingPage()
{
    delete ui;
}

void StatisticsProcessingPage::initializePage()
{
    QTimer::singleShot(0, this, [this] {
        QWizard *wizard = this->wizard();
        wizard->button(QWizard::BackButton)->setEnabled(false);
        wizard->button(QWizard::CommitButton)->setText(tr("Next"));
    });
}

void StatisticsProcessingPage::cleanupPage()
{
    if (m_timer.isActive())
        m_timer.stop();
}

bool StatisticsProcessingPage::isComplete() const
{
    return m_thread == nullptr && !m_timer.isActive();
}

void StatisticsProcessingPage::process(const QDate &date, const QString &outputDir, const QStringList &files, AbstractKitManager *kitManager, bool withStats2)
{
    m_thread = QThread::create([date, outputDir, files, kitManager, withStats2] {
        QDir dir(outputDir);
        if (!dir.exists())
            dir.mkpath(".");

        Package package = readPackage(files);
        package.setDate(date);
        writeEnveloppes(package.enveloppes(), dir.absolutePath(), kitManager);
        writeStats(package, dir.absolutePath(), kitManager, withStats2);
    });

    connect(m_thread, &QThread::finished, &m_timer, QOverload<>::of(&QTimer::start));
    connect(m_thread, &QThread::finished, this, [this] {
        m_thread = nullptr;
        m_timer.start();
    });

    m_thread->start();
}

Package StatisticsProcessingPage::readPackage(const QStringList &files)
{
    EnveloppeReader reader;

    Package package;
    for (const QString &file : files) {
        const Enveloppe enveloppe = reader.readEnveloppe(file);
        package.addEnveloppe(enveloppe);
    }
    return package;
}

void StatisticsProcessingPage::writeEnveloppes(const QList<Enveloppe> &enveloppes, const QString &dir, AbstractKitManager *kitManager)
{
    EnveloppeWriter writer(kitManager);
    for (const Enveloppe &enveloppe : enveloppes)     {
        writer.writeEnveloppe(enveloppe, dir + '/' + enveloppe.number() + ".xlsx");
    }
}

Statistics StatisticsProcessingPage::writeStats(const Package &package, const QString &dir, AbstractKitManager *kitManager, bool withStats2)
{
    StatisticsCalculator calculator(kitManager);
    const Statistics regularStats = calculator.compute(package, StatisticsCalculator::NoOptions);

    StatisticsWritter writer;
    writer.writeStatistics(regularStats, dir + "/STATISTIQUES.xlsx");

    if (withStats2) {
        const Statistics extraStats = calculator.compute(package, StatisticsCalculator::ExcludeKownKits | StatisticsCalculator::IncludeUnknownKits);
        writer.writeStatistics(extraStats, dir + "/STATISTIQUES2.xlsx");
    }

    return regularStats;
}
