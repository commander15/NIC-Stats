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

    //return test(&kitManager);

    StatisticsComputingWizard wizard;
    wizard.setKitManager(&kitManager);
    return wizard.exec();

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
    QStringList files;
    files.append("EN01_1767.xlsx");
    files.append("EN01_1798.xlsx");
    files.append("EN01_1823.xlsx");
    files.append("EN01_1829.xlsx");
    files.append("EN01_1832.xlsx");
    files.append("EN01_1903.xlsx");
    files.append("EN01_BIS 1719.xlsx");
    files.append("EN01_BIS1761.xlsx");
    files.append("EN01_BIS1762.xlsx");
    files.append("EN01_BIS1763.xlsx");
    files.append("EN01_BIS1764.xlsx");
    files.append("EN01_BIS1765.xlsx");
    files.append("EN01_BIS1777.xlsx");
    files.append("EN01_BIS1780.xlsx");
    files.append("EN01_BIS1781.xlsx");
    files.append("EN01_BIS1782.xlsx");
    files.append("EN01_BIS1783.xlsx");
    files.append("EN01_BIS1784.xlsx");
    files.append("EN01_BIS 1790.xlsx");
    files.append("EN01_BIS 1792.xlsx");
    files.append("EN01_BIS1793.xlsx");
    files.append("EN01_BIS1794.xlsx");
    files.append("EN01_BIS1795.xlsx");
    files.append("EN01_BIS1796.xlsx");
    files.append("EN01_BIS1812.xlsx");
    files.append("EN01_BIS1813.xlsx");
    files.append("EN01_BIS1814.xlsx");
    files.append("EN01_BIS1819.xlsx");
    files.append("EN01_BIS1820.xlsx");
    files.append("EN01_BIS1821.xlsx");
    files.append("EN01_BIS1830.xlsx");
    files.append("EN01_BIS1831.xlsx");
    files.append("EN01_BIS1836.xlsx");
    files.append("EN01_BIS1837.xlsx");
    files.append("EN01_BIS 1838.xlsx");
    files.append("EN01_BIS 1839.xlsx");
    files.append("EN01_BIS1840.xlsx");
    files.append("EN01_BIS1847.xlsx");
    files.append("EN01_BIS1852.xlsx");
    files.append("EN01_BIS1853.xlsx");
    files.append("EN01_BIS1854.xlsx");
    files.append("EN01_BIS1855.xlsx");
    files.append("EN01_BIS1856.xlsx");
    files.append("EN01_BIS1857.xlsx");
    files.append("EN01_BIS1858.xlsx");
    files.append("EN01_BIS1868.xlsx");
    files.append("EN01_BIS1869.xlsx");
    files.append("EN01_BIS1872.xlsx");
    files.append("EN01_BIS1874.xlsx");
    files.append("EN01_BIS1876.xlsx");
    files.append("EN01_BIS1877.xlsx");
    files.append("EN01_BIS1878.xlsx");
    files.append("EN01_BIS1879.xlsx");
    files.append("EN01_BIS1880.xlsx");
    files.append("EN01_BIS1881.xlsx");
    files.append("EN01_BIS1885.xlsx");
    files.append("EN01_BIS1886.xlsx");
    files.append("EN01_BIS1887.xlsx");
    files.append("EN01_BIS1888.xlsx");
    files.append("EN01_BIS1890.xlsx");
    files.append("EN01_BIS1898.xlsx");
    files.append("EN01_BIS1900.xlsx");
    files.append("EN01_BIS1901.xlsx");
    files.append("EN01_BIS1902.xlsx");
    files.append("EN01_BIS1911.xlsx");
    files.append("EN01_BIS1912.xlsx");
    files.append("EN01_BIS1913.xlsx");
    files.append("EN01_BIS1914.xlsx");
    files.append("EN01_BIS1915.xlsx");
    files.append("EN01_BIS1916.xlsx");
    files.append("EN01_BIS1917.xlsx");
    files.append("EN01_BIS1921.xlsx");
    files.append("EN01_BIS1934.xlsx");
    files.append("EN01_BIS1935.xlsx");
    files.append("EN01_BIS1936.xlsx");
    files.append("EN01_BIS1937.xlsx");
    files.append("EN01_BIS1938.xlsx");
    files.append("EN_BIS1889.xlsx");

    const QString outputDir = "/home/commander/Desktop/NEW";
    for (const QString &file : std::as_const(files)) {
        Enveloppe e = reader.readEnveloppe("/home/commander/Downloads/WhatSie/EN01BIS NEW/" + file, &readerError);
        package.addEnveloppe(e);
        writer.writeEnveloppe(e, outputDir + '/' + e.number() + ".xlsx");
    }

    // Computing and writing stats
    stats.writeStatistics(package, "/home/commander/Desktop/NEW/statistiques.xlsx", &statsError);

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
