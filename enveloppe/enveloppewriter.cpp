#include "enveloppewriter.h"

#include <common/error.h>

#include <kit/abstractkitmanager.h>

#include <enveloppe/record.h>

#include <xlsxdocument.h>

EnveloppeWriter::EnveloppeWriter(AbstractKitManager *kitManager)
    : m_recapSheetName("RECAP")
    , m_kitManager(kitManager)
{
}

void EnveloppeWriter::writeEnveloppe(const Enveloppe &enveloppe, const QString &fileName, Error *error)
{
    QXlsx::Document *document = openFile(fileName, QIODevice::WriteOnly, error);
    if (document == nullptr) return;

    // Cleaning all sheets (if exists)
    deleteAllSheets(document);

    // Renaming the main sheet (RECAP)
    document->renameSheet(document->currentSheet()->sheetName(), m_recapSheetName);

    // Identifying kit numbers
    const QStringList kitNumbers = m_kitManager->kitsNumbers();

    // Generating recap sheet
    int row = 1;
    for (const QString &kitNumber : kitNumbers) {
        const Kit kit = m_kitManager->kitNumbered(kitNumber);
        row = writeTable(enveloppe, kit, document, row + 2, error);
    }

    // Generating kit based sheets
    for (const QString &kitNumber : kitNumbers) {
        const Kit kit = m_kitManager->kitNumbered(kitNumber);

        const QString sheetName = kitNumber + " - " + kit.name();
        if (!document->addSheet(sheetName)) {
            if (error != nullptr) {
                *error = Error(1, "Can't create sheet for kit number " + kitNumber);
            }

            deleteAllSheets(document);
            return;
        }

        document->selectSheet(sheetName);
        writeTable(enveloppe, kit, document, 1, error);
    }

    closeFile(document);
}

int EnveloppeWriter::writeTable(const Enveloppe &enveloppe, const Kit &kit, QXlsx::Document *document, int startRow, Error *error)
{
    const int headerRow = writeTableHeader(enveloppe, kit, document, startRow);
    return writeTableContent(enveloppe, enveloppe.kitRecords(kit.regularExpression()), document, headerRow + 1);
}

int EnveloppeWriter::writeTableHeader(const Enveloppe &enveloppe, const Kit &kit, QXlsx::Document *document, int row)
{
    // Data
    const QString text = enveloppe.number() + " - " + kit.name();

    // Formating title
    QXlsx::Format format;
    format.setBorderStyle(QXlsx::Format::BorderStyle::BorderDouble);
    format.setFontSize(14);
    format.setFontBold(true);
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format.setVerticalAlignment(QXlsx::Format::AlignVCenter);

    // Merging columns (1 to 6)
    document->mergeCells(QXlsx::CellRange(row, 1, row, 6), format);

    // Filling
    document->write(row, 1, text, format);

    // Formating headers
    format.setFontSize(12);

    // Adding columns headers
    ++row;
    document->write(row, 1, "No. Personal No.", format);
    document->write(row, 2, "Document No.", format);
    document->write(row, 3, "Enrollment No.", format);
    document->write(row, 4, "Surname, Given Names", format);
    document->write(row, 5, "Sex", format);
    document->write(row, 6, "Date of Birth", format);

    // Resizing columns
    document->setColumnWidth(1, 18);
    document->setColumnWidth(2, 18);
    document->setColumnWidth(3, 18);
    document->setColumnWidth(4, 40);
    document->setColumnWidth(5, 6);
    document->setColumnWidth(6, 18);

    return row;
}

int EnveloppeWriter::writeTableContent(const Enveloppe &enveloppe, const QList<Record> records, QXlsx::Document *document, int startRow)
{
    int row = startRow;

    QXlsx::Format format;
    format.setBorderStyle(QXlsx::Format::BorderDouble);
    format.setFontSize(11);
    format.setFontBold(true);

    for (const Record &record : records) {
        document->write(row, 1, record.personalNumber(), format);
        document->write(row, 2, record.documentNumber(), format);
        document->write(row, 3, record.enrollmentNumber(), format);
        document->write(row, 4, record.fullName(", "), format);
        document->write(row, 5, record.sex(), format);
        document->write(row, 6, record.birthdate(), format);
        ++row;
    }

    document->write(row, 1, "TOTAL", format);
    document->write(row, 2, records.count(), format);

    return row;
}
