#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmessagebox.h>

#include <QtCore/qsettings.h>

#include <kit/kitmanager.h>

#include <stats-wizard/statisticscomputingwizard.h>

#include "test.h"

void showExpiryNotice();
void loadKits(AbstractKitManager &manager, QSettings &settings);
void saveKits(AbstractKitManager &manager, QSettings &settings);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("NIC-Stats");
    app.setApplicationVersion(APP_VERSION);
    app.setOrganizationName("AUGENTIC");
    app.setApplicationDisplayName("NIC Stats");
    app.setStyle("fusion");

    if (QDate::currentDate() > QDate(2025, 12, 1)) {
        showExpiryNotice();
        return 0;
    }

    QSettings settings;
    settings.sync();

    KitManager kitManager;
    loadKits(kitManager, settings);

    int exitCode;
    if (argc >= 4 && strcmp(argv[1], "--test") == 0) {
        QString testInputDir(argv[2]);
        QString testOutputDir(argv[3]);
        exitCode = test(testInputDir, testOutputDir, &kitManager);
    } else {
        StatisticsComputingWizard wizard;
        wizard.setKitManager(&kitManager);
        exitCode = wizard.exec();
    }

    // We save kits in settings before exit
    saveKits(kitManager, settings);
    settings.sync();

    return exitCode;
}

void showExpiryNotice()
{
    QString msg = "App requires an update, if you don't understand what is happening, please, reach out using one of the following ways:\n\n";
    msg.append("**Amadou Benjamain**\n\n+237 691 272 717 (Whatsapp + Mobile)\namadoubenjamain@gmail.com\n\n");
    msg.append("\n\nThanks for using NIC Stats 😊");
    QMessageBox box;
    box.setWindowTitle("Update required !");
    box.setIcon(QMessageBox::Information);
    box.setText(msg);
    box.setTextFormat(Qt::MarkdownText);
    box.exec();
}

void loadKits(AbstractKitManager &manager, QSettings &settings)
{
    settings.beginGroup("KITS");
    bool save = false;

    const QStringList kits = settings.childGroups();
    // If there is no kits, we add using hardcoded values
    if (kits.isEmpty()) {
        manager.setOfficeKit(Kit(QRegularExpression(R"(^03..$)"), "EN01BIS", Kit::DesktopKit));
        //kitManager.addKit(Kit(QRegularExpression(R"(^0058|0060$)"), "EN01", Kit::DesktopKit));
        manager.addKit(Kit("0439", "MEME", Kit::MobileKit));
        manager.addKit(Kit("0515", "BLANGOUA", Kit::MobileKit));
        manager.addKit(Kit("0506", "MAROUA KIT MOBILE", Kit::MobileKit));
        manager.addKit(Kit("0516", "MAGA", Kit::MobileKit));
        manager.addKit(Kit("0518", "ZINA", Kit::MobileKit));
        manager.addKit(Kit("0423", "KOUSSERI", Kit::MobileKit));
        manager.addKit(Kit("0514", "WAZA", Kit::MobileKit));
        manager.addKit(Kit("0426", "MAKARY", Kit::MobileKit));
        manager.addKit(Kit("0443", "GOULFEY", Kit::MobileKit));
        manager.addKit(Kit("0427", "MOKONG", Kit::MobileKit));
        manager.addKit(Kit("0511", "MOKOLO", Kit::MobileKit));
        manager.addKit(Kit("0424", "YAGOUA", Kit::MobileKit));
        manager.addKit(Kit("0440", "KAELE", Kit::MobileKit));
        manager.addKit(Kit("0517", "DARAK", Kit::MobileKit));
        manager.addKit(Kit("0441", "MORA", Kit::MobileKit));
        save = true;
    } else {
        // We load from settings
        QMap<int, Kit> mappedKits;

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

            mappedKits.insert(settings.value("INDEX").toInt(), kit);

            settings.endGroup();
        }

        if (!mappedKits.isEmpty()) {
            manager.setOfficeKit(mappedKits.first());
            manager.addKits(mappedKits.values());
        }
    }

    settings.endGroup(); // KITS

    if (save)
        saveKits(manager, settings);
}

void saveKits(AbstractKitManager &manager, QSettings &settings)
{
    settings.remove("KITS");
    settings.beginGroup("KITS");

    int index = 0; // To keep order
    const QStringList kits = manager.kitsNames();
    for (const QString &kitName : kits) {
        const Kit kit = manager.kitNamed(kitName);
        settings.beginGroup(kitName);
        settings.setValue("INDEX", index++);
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

    settings.endGroup(); // KITS
}
