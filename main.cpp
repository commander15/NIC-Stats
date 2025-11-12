#include <QtWidgets/qapplication.h>

#include <QtCore/qsettings.h>

#include <kit/kitmanager.h>

#include <stats-wizard/statisticscomputingwizard.h>

#include "test.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("NIC-Stats");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("AUGENTIC");
    app.setApplicationDisplayName("NIC Stats");
    app.setStyle("fusion");

    QSettings settings;
    settings.sync();

    KitManager kitManager;

    settings.beginGroup("KITS");
    QStringList kits = settings.childGroups();

    // If there is no kits, we add using hardcoded values
    if (kits.isEmpty() || true) {
        kitManager.setOfficeKit(Kit(QRegularExpression(R"(^03.+$)"), "EN01BIS", Kit::DesktopKit));
        //kitManager.addKit(Kit(QRegularExpression(R"(^0058|0060$)"), "EN01", Kit::DesktopKit));
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
        kitManager.addKit(Kit("0517", "DARAK", Kit::MobileKit));
        kitManager.addKit(Kit("0441", "MORA", Kit::MobileKit));

        // We save kits in settings
        kits = kitManager.kitsNames();
        for (const QString &kitName : std::as_const(kits)) {
            const Kit kit = kitManager.kitNamed(kitName);
            settings.beginGroup(kitName);
            settings.setValue("NAME", kitName);
            settings.setValue("EXPRESSION", kit.regularExpression().pattern());

            switch (kit.type()) {
            case Kit::DesktopKit:
                settings.setValue("TYPE", "DESKTOP");
                break;

            case Kit::MobileKit:
                settings.setValue("TYPE", "MOBILE");
                break;

            default:
                settings.setValue("TYPE", QVariant());
            }

            settings.endGroup();
        }
    } else {
        // We load from settings

        for (const QString &kitName : std::as_const(kits)) {
            settings.beginGroup(kitName);

            Kit kit;
            kit.setName(settings.value("NAME").toString());
            kit.setRegularExpression(QRegularExpression(settings.value("EXPRESSION").toString()));

            const QString type = settings.value("TYPE").toString();
            if (type == "DESKTOP") {
                kit.setType(Kit::DesktopKit);
            } else if (type == "MOBILE") {
                kit.setType(Kit::MobileKit);
            } else {
                kit.setType(Kit::UnknownKit);
            }

            kitManager.addKit(kit);

            settings.endGroup();
        }
    }

    settings.endGroup();

    if (argc >= 4 && strcmp(argv[1], "--test") == 0) {
        QString testInputDir(argv[2]);
        QString testOutputDir(argv[3]);
        return test(testInputDir, testOutputDir, &kitManager);
    } else {
        StatisticsComputingWizard wizard;
        wizard.setKitManager(&kitManager);
        return wizard.exec();
    }
}
