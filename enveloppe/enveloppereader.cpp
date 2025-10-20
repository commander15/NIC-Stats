#include "enveloppereader.h"

#include <common/error.h>
#include <enveloppe/record.h>

#include <QRegularExpression>
#include <QDate>

#include <xlsxdocument.h>

EnveloppeReader::EnveloppeReader()
{
}

Enveloppe EnveloppeReader::readEnveloppe(const QString &fileName, Error *error)
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
        enveloppe.addRecord(record, record.enrollmentNumber().section('-', 0, 0));
    }

    closeFile(doc);
    return enveloppe;
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
