#ifndef PACKAGE_H
#define PACKAGE_H

#include <QSharedDataPointer>

class Enveloppe;

class QDate;

class PackageData;
class Package
{
public:
    Package();
    Package(const Package &);
    Package(Package &&);
    Package &operator=(const Package &);
    Package &operator=(Package &&);
    ~Package();

    QDate date() const;
    void setDate(const QDate &date);

    QStringList enveloppesNumbers() const;
    QList<Enveloppe> enveloppes() const;
    void addEnveloppe(const Enveloppe &enveloppe);

private:
    QSharedDataPointer<PackageData> data;
};

#endif // PACKAGE_H
