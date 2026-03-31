#include "enveloppegenerator.h"

#include <data/enveloppe.h>
#include <data/record.h>

#include <qregularexpression.h>
#include <xlsxdocument.h>

EnveloppeGenerator::EnveloppeGenerator()
{
    m_kits.insert("0439", "MEME");
    m_kits.insert("0515", "BLANGOUA");
    m_kits.insert("0506", "MAROUA KIT MOBILE");
    m_kits.insert("0516", "MAGA");
    m_kits.insert("0518", "ZINA");
    m_kits.insert("0423", "KOUSSERI");
    m_kits.insert("0514", "WAZA");
    m_kits.insert("0426", "MAKARY");
    m_kits.insert("0443", "GOULFEY");
    m_kits.insert("0427", "MOKONG");
    m_kits.insert("0511", "MOKOLO");
    m_kits.insert("0424", "YAGOUA");
    m_kits.insert("0440", "KAELE");
}

void EnveloppeGenerator::generate(const QString &folder, const Enveloppe &enveloppe, Error *error)
{
    QXlsx::Document *doc = openFile(folder + '/' + enveloppe.number(), error);
    if (doc == nullptr) return;

    // Clear any existing sheets
    const QStringList sheetNames = doc->sheetNames();
    for (const QString &sheetName : sheetNames)
        doc->deleteSheet(sheetName);

    // Finding kits
    const QStringList kits = enveloppe.recordsKits(QRegularExpression(R"(03.+)"));

    // Creating Recap sheet
    doc->renameSheet(doc->currentSheet()->sheetName(), "RECAP");
    int lastRow = generateKitBasedTable(QRegularExpression(R"(^03.+$)"), enveloppe, doc);
    for (const QString &kit : kits) {
        lastRow = generateKitBasedTable(QRegularExpression(kit), enveloppe, doc, lastRow + 2);
    }

    // Creating office sheet
    const QString sheetName = enveloppe.office();
    doc->addSheet(sheetName);
    doc->selectSheet(sheetName);
    generateKitBasedTable(QRegularExpression(R"(03.+)"), enveloppe, doc);

    // Creating KIT-based sheets
    for (const QString &kit : kits) {
        const QString sheetName = kit + " - " + kitName(kit, enveloppe);
        doc->addSheet(sheetName);
        doc->selectSheet(sheetName);
        generateKitBasedTable(QRegularExpression(kit), enveloppe, doc);
    }

    doc->save();
}

int EnveloppeGenerator::generateKitBasedTable(const QRegularExpression &kit, const Enveloppe &enveloppe, QXlsx::Document *doc, int startRow)
{
    const QList<Record> records = enveloppe.kitRecords(kit);

    int i = startRow;

    QXlsx::Format format;
    format.setFont(QFont(QString(), 14, QFont::Bold));
    format.setVerticalAlignment(QXlsx::Format::VerticalAlignment::AlignVCenter);
    format.setHorizontalAlignment(QXlsx::Format::HorizontalAlignment::AlignHCenter);

    // Generating header
    doc->mergeCells(QXlsx::CellRange(i, 1, 1, 6));
    doc->write(i, 1, enveloppe.number() + " - " + kitName(records.first().enrollmentNumber(), enveloppe), format);

    // Writing records
    i++;
    for (const Record &record : records) {
        record.save(doc, i);
        ++i;
    }

    // Writing total
    doc->write(i, 1, "TOTAL", format);
    doc->write(i, 2, records.size(), format);

    return i;
}

QString EnveloppeGenerator::kitName(const QString &enrollmentNumber, const Enveloppe &enveloppe) const
{
    const QString kitNumber = enrollmentNumber.section('-', 0, 0);
    return m_kits.value(kitNumber, enveloppe.office());
}
