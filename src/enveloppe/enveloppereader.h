#ifndef ENVELOPPEREADER_H
#define ENVELOPPEREADER_H

#include <common/reader.h>
#include <enveloppe/enveloppe.h>

class EnveloppeReader : public Reader
{
public:
    struct HeaderData {
        QString office;
        QString batchNumber;
        int count = 0;
    };

    EnveloppeReader();

    Enveloppe readEnveloppe(const QString &fileName, Error *error = nullptr);

    HeaderData readHeaderData(QXlsx::Document *doc, Error *error = nullptr) const;
    Record readRecord(int row, QXlsx::Document *doc, Error *error = nullptr) const;
    QList<Record> readRecords(int count, QXlsx::Document *doc, Error *error = nullptr) const;

private:
    Enveloppe readExcelEnveloppe(const QString &fileName, Error *error = nullptr);
    Enveloppe readPdfEnveloppe(const QString &fileName, Error *error = nullptr);
};

#endif // ENVELOPPEREADER_H
