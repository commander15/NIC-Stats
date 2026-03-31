#include "enveloppereader.h"

#include <common/error.h>
#include <enveloppe/record.h>

#include <QRegularExpression>
#include <QDate>

#ifdef QT_PDF_LIB
#   include <QPdfDocument>
#endif

#include <xlsxdocument.h>

EnveloppeReader::EnveloppeReader()
{
}

Enveloppe EnveloppeReader::readEnveloppe(const QString &fileName, Error *error)
{
    if (fileName.endsWith(".xlsx"))
        return readExcelEnveloppe(fileName, error);

#ifdef QT_PDF_LIB
    if (fileName.endsWith(".pdf"))
        return readPdfEnveloppe(fileName, error);
#endif

    if (error)
        *error = Error(100, "Unknown file extension");
    return Enveloppe();
}

Record EnveloppeReader::readRecord(int row, QXlsx::Document *doc, Error *error) const
{
    Record record;
    const QStringList fullName = doc->read(row, 4).toString().split(",", Qt::SkipEmptyParts);

    record.setPersonalNumber(doc->read(row, 1).toString());
    record.setDocumentNumber(doc->read(row, 2).toString());
    record.setEnrollmentNumber(doc->read(row, 3).toString());
    record.setName(fullName.first());
    record.setFirstName(fullName.size() == 2 ? fullName.last() : QString());
    record.setSex(doc->read(row, 5).toString());

    const QString birthdateValue = doc->read(row, 6).toString();

    static const QRegularExpression expr(R"(^\d+.\d+$)");

    const QRegularExpressionMatch match = expr.match(birthdateValue);
    if (match.hasMatch()) {
        static const QDate baseDate(1899, 12, 30);
        const QDate date = baseDate.addDays(static_cast<int>(birthdateValue.toDouble()));
        record.setBirthdate(date.toString("dd/MM/yyyy"));
    } else {
        record.setBirthdate(birthdateValue);
    }

    return record;
}

EnveloppeReader::HeaderData EnveloppeReader::readHeaderData(QXlsx::Document *doc, Error *error) const
{
    if (!doc->selectSheet("Table 1")) {
        if (error != nullptr) {
            *error = Error(1, "'Table 1' sheet not found for header data extraction !");
        }
        return HeaderData();
    }

    const QStringList headerData = doc->read(1, 1).toString().split('\n');

    return {
        .office = headerData.at(4).section(' ', -1),
        .batchNumber = headerData.at(5).section(' ', -1),
        .count = headerData.at(7).section(':', 1, 1).section(' ', 0, 0, QString::SectionSkipEmpty).toInt(),
    };
}

QList<Record> EnveloppeReader::readRecords(int count, QXlsx::Document *doc, Error *error) const
{
    if (!doc->selectSheet("Table 2")) {
        if (error != nullptr) {
            *error = Error(10, "'Table 2' sheet not found for record data !");
        }
        return {};
    }

    QList<Record> records;

    // We get records
    for (int i(0); i < count; ++i) {
        records.append(readRecord(i + 1, doc, error));
    }

    return records;
}

Enveloppe EnveloppeReader::readExcelEnveloppe(const QString &fileName, Error *error)
{
    QXlsx::Document *doc = openFile(fileName, error);
    if (doc == nullptr) return Enveloppe();

    Enveloppe enveloppe;

    // Extracting header data
    const HeaderData header = readHeaderData(doc, error);
    enveloppe.setNumber(header.batchNumber);
    enveloppe.setOffice(header.office);

    // Extracting records
    const QList<Record> records = readRecords(header.count, doc, error);
    for (const Record &record : records) {
        enveloppe.addRecord(record);
    }

    closeFile(doc);
    return enveloppe;
}

Enveloppe EnveloppeReader::readPdfEnveloppe(const QString &fileName, Error *error)
{
#ifdef QT_PDF_LIB
    QPdfDocument doc;
    if (doc.load(fileName) != QPdfDocument::Error::None) {
        if (error)
            *error = Error(static_cast<int>(doc.error()));
        return Enveloppe();
    }

    const QStringList content = doc.getAllText(0).text().split("\r\n");

    Enveloppe enveloppe;
    enveloppe.setNumber(content.at(7));
    enveloppe.setOffice(content.at(9).mid(8));

    static QList<std::function<Record(const QString &data)>> parsers;
    if (parsers.empty()) {
        auto normal = [](const QString &data) {
            static const QRegularExpression expr(R"(^\d{1,2} ([A-Z0-9]{10}) (\d{9}) (\d{4}-\d{6}) (.*) ([MF]) (.{10})$)");
            const QRegularExpressionMatch match = expr.match(data);

            Record record;
            if (match.hasMatch()) {
                record.setPersonalNumber(match.captured(1));
                record.setDocumentNumber(match.captured(2));
                record.setEnrollmentNumber(match.captured(3));
                record.setFullName(match.captured(4));
                record.setSex(match.captured(5));
                record.setBirthdate(match.captured(6));
            }
            return record;
        };
        parsers.append(normal);

        auto abnormal = [](const QString &data) {
            static const QRegularExpression expr(R"(^\d{1,2} (\d{9}) ([A-Z0-9]{10}) (\d{4}-\d{6}) (.*) ([MF]) (.{10})$)");
            const QRegularExpressionMatch match = expr.match(data);

            Record record;
            if (match.hasMatch()) {
                record.setPersonalNumber(match.captured(2));
                record.setDocumentNumber(match.captured(1));
                record.setEnrollmentNumber(match.captured(3));
                record.setFullName(match.captured(4));
                record.setSex(match.captured(5));
                record.setBirthdate(match.captured(6));
            }
            return record;
        };
        parsers.append(abnormal);
    }

    const int count = content.at(6).right(2).toInt();
    for (int i(0); i < count; ++i) {
        const QString data = content.at(12 + i);

        for (const auto &parser : std::as_const(parsers)) {
            Record record = parser(data);
            if (!record.name().isEmpty()) {
                enveloppe.addRecord(record);
                break;
            }
        }
    }

    return enveloppe;
#else
    return Enveloppe();
#endif
}
