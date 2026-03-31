#ifndef ENVELOPPEWRITER_H
#define ENVELOPPEWRITER_H

#include <common/writer.h>

#include <enveloppe/enveloppe.h>

class AbstractKitManager;
class Kit;

class EnveloppeWriter : public Writer
{
public:
    EnveloppeWriter(AbstractKitManager *kitManager);

    void writeEnveloppe(const Enveloppe &enveloppe, const QString &fileName, Error *error = nullptr);

    int writeTable(const Enveloppe &enveloppe, const Kit &kit, QXlsx::Document *document, int startRow = 1, Error *error = nullptr);
    int writeTableHeader(const Enveloppe &enveloppe, const Kit &kit, QXlsx::Document *document, int row = 1);
    int writeTableContent(const Enveloppe &enveloppe, const QList<Record> records, QXlsx::Document *document, int startRow = 2);

private:
    QString m_recapSheetName;
    AbstractKitManager *m_kitManager;
};

#endif // ENVELOPPEWRITER_H
