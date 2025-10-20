#ifndef ENVELOPPE_H
#define ENVELOPPE_H

#include <QSharedDataPointer>

class Record;

class EnvelopData;
class Enveloppe
{
public:
    Enveloppe();
    Enveloppe(const Enveloppe &);
    Enveloppe(Enveloppe &&);
    Enveloppe &operator=(const Enveloppe &);
    Enveloppe &operator=(Enveloppe &&);
    ~Enveloppe();

    QString number() const;
    void setNumber(const QString &number);

    QString office() const;
    void setOffice(const QString &office);

    QList<Record> kitRecords(const QString &kit) const;
    QList<Record> kitRecords(const QRegularExpression &kit) const;
    QStringList recordsKits(const QRegularExpression &officeKit) const;
    void addRecord(const Record &record, const QString &kit);

private:
    QSharedDataPointer<EnvelopData> data;

    friend class EnveloppeReader;
    friend class EnveloppeWriter;
};

#endif // ENVELOPPE_H
