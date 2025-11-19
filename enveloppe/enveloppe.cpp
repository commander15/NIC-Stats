#include "enveloppe.h"

#include <common/error.h>

#include <enveloppe/record.h>

#include <QList>
#include <QRegularExpression>

class EnvelopData : public QSharedData
{
public:
    QString number;
    QString office;
    QMultiMap<QString, Record> records;
};

Enveloppe::Enveloppe()
    : data(new EnvelopData)
{}

Enveloppe::Enveloppe(const Enveloppe &rhs)
    : data{rhs.data}
{}

Enveloppe::Enveloppe(Enveloppe &&rhs)
    : data{std::move(rhs.data)}
{}

Enveloppe &Enveloppe::operator=(const Enveloppe &rhs)
{
    if (this != &rhs)
        data = rhs.data;
    return *this;
}

Enveloppe &Enveloppe::operator=(Enveloppe &&rhs)
{
    if (this != &rhs)
        data = std::move(rhs.data);
    return *this;
}

Enveloppe::~Enveloppe() {}

QString Enveloppe::number() const
{
    return data->number;
}

void Enveloppe::setNumber(const QString &number)
{
    data->number = number;
}

QString Enveloppe::office() const
{
    return data->office;
}

void Enveloppe::setOffice(const QString &office)
{
    data->office = office;
}

QList<Record> Enveloppe::kitRecords(const QString &kit) const
{
    return kitRecords(QRegularExpression("^" + kit + "$"));
}

QList<Record> Enveloppe::kitRecords(const QRegularExpression &kit) const
{
    QList<Record> records;

    QStringList kitNumbers = data->records.keys();
    kitNumbers.removeDuplicates();
    kitNumbers.removeIf([&kit](const QString &number) {
        const QRegularExpressionMatch match = kit.match(number);
        return !match.hasMatch();
    });

    for (const QString &kitNumber : std::as_const(kitNumbers))
        records.append(data->records.values(kitNumber));

    return records;
}

QStringList Enveloppe::recordsKits(const QRegularExpression &officeKit) const
{
    QStringList kits = data->records.keys();
    kits.removeDuplicates();
    kits.removeIf([&officeKit](const QString &kit) {
        return officeKit.match(kit).hasMatch();
    });
    return kits;
}

QStringList Enveloppe::recordsKitsNumbers() const
{
    QStringList numbers = data->records.keys();
    numbers.removeDuplicates();
    return numbers;
}

QList<Record> Enveloppe::records() const
{
    return data->records.values();
}

void Enveloppe::addRecord(const Record &record)
{
    const QString kit = record.enrollmentNumber().section('-', 0, 0);
    data->records.insert(kit, record);
}
