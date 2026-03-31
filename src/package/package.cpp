#include "package.h"

#include <enveloppe/enveloppe.h>

#include <xlsxworkbook.h>

#include <QList>
#include <QDate>

class PackageData : public QSharedData
{
public:
    QDate date;
    QList<Enveloppe> enveloppes;
};

Package::Package()
    : data(new PackageData)
{
}

Package::Package(const Package &rhs)
    : data{rhs.data}
{}

Package::Package(Package &&rhs)
    : data{std::move(rhs.data)}
{}

Package &Package::operator=(const Package &rhs)
{
    if (this != &rhs)
        data = rhs.data;
    return *this;
}

Package &Package::operator=(Package &&rhs)
{
    if (this != &rhs)
        data = std::move(rhs.data);
    return *this;
}

Package::~Package()
{
}

QDate Package::date() const
{
    return data->date;
}

void Package::setDate(const QDate &date)
{
    data->date = date;
}

QStringList Package::enveloppesNumbers() const
{
    QStringList numbers;
    numbers.resize(data->enveloppes.size());

    std::transform(data->enveloppes.begin(), data->enveloppes.end(), numbers.begin(), [](const Enveloppe &enveloppe) {
        return enveloppe.number();
    });

    return numbers;
}

QList<Enveloppe> Package::enveloppes() const
{
    return data->enveloppes;
}

void Package::addEnveloppe(const Enveloppe &enveloppe)
{
    auto it = std::find_if(data->enveloppes.begin(), data->enveloppes.end(), [enveloppe](const Enveloppe &existing) {
        return enveloppe.number() == existing.number();
    });

    if (it == data->enveloppes.end())
        data->enveloppes.append(enveloppe);
    else
        data->enveloppes.replace(std::distance(data->enveloppes.begin(), it), enveloppe);
}
