#include "package.h"

#include <enveloppe/enveloppe.h>

#include <xlsxworkbook.h>

#include <QList>

class PackageData : public QSharedData
{
public:
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
    data->enveloppes.append(enveloppe);
}
