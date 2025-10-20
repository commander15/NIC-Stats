#include <QtWidgets/qapplication.h>

#include <kit/kitmanager.h>

#include <stats-wizard/statisticscomputingwizard.h>

int test(AbstractKitManager *kitManager);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationDisplayName("NIC Stats");
    app.setStyle("fusion");

    // We hard code kits for now
    KitManager kitManager;
    kitManager.addKit(Kit(QRegularExpression(R"(^03.+$)"), "EN01BIS", Kit::DesktopKit));
    kitManager.addKit(Kit("0439", "MEME", Kit::MobileKit));
    kitManager.addKit(Kit("0515", "BLANGOUA", Kit::MobileKit));
    kitManager.addKit(Kit("0506", "MAROUA KIT MOBILE", Kit::MobileKit));
    kitManager.addKit(Kit("0516", "MAGA", Kit::MobileKit));
    kitManager.addKit(Kit("0518", "ZINA", Kit::MobileKit));
    kitManager.addKit(Kit("0423", "KOUSSERI", Kit::MobileKit));
    kitManager.addKit(Kit("0514", "WAZA", Kit::MobileKit));
    kitManager.addKit(Kit("0426", "MAKARY", Kit::MobileKit));
    kitManager.addKit(Kit("0443", "GOULFEY", Kit::MobileKit));
    kitManager.addKit(Kit("0427", "MOKONG", Kit::MobileKit));
    kitManager.addKit(Kit("0511", "MOKOLO", Kit::MobileKit));
    kitManager.addKit(Kit("0424", "YAGOUA", Kit::MobileKit));
    kitManager.addKit(Kit("0440", "KAELE", Kit::MobileKit));

    StatisticsComputingWizard wizard;
    wizard.setKitManager(&kitManager);
    return wizard.exec();

    return test(&kitManager);

    return app.exec();
}

// --- TESTING ---

#include <common/error.h>
#include <statistics/statisticswritter.h>
#include <enveloppe/enveloppereader.h>
#include <enveloppe/enveloppewriter.h>

int test(AbstractKitManager *kitManager) {
    EnveloppeReader reader;
    Error readerError;

    EnveloppeWriter writer(kitManager);
    Error writerError;

    StatisticsWritter stats(kitManager);
    Error statsError;

    Package package;

    // Reading enveloppes
    Enveloppe e = reader.readEnveloppe("/home/commander/Downloads/Documents/EN01 _BIS1697_1.xlsx", &readerError);
    package.addEnveloppe(e);
    e = reader.readEnveloppe("/home/commander/Downloads/Documents/EN01_BIS 1753_1.xlsx", &readerError);
    package.addEnveloppe(e);

    // Writing enveloppe
    writer.writeEnveloppe(e, "/home/commander/Desktop/EN-TEST/" + e.number() + ".xlsx", &writerError);

    // Computing and writing stats
    stats.writeStatistics(package, "/home/commander/Desktop/EN-TEST/statistiques.xlsx", &statsError);

    // Checking errors

    if (readerError.code() != 0) {
        QTextStream(stdout) << "READ: " << readerError.errorString() << Qt::endl;
        return 1;
    }

    if (writerError.code() != 0) {
        QTextStream(stdout) << "WRITE: " << writerError.errorString() << Qt::endl;
        return 1;
    }

    return 0;
}
