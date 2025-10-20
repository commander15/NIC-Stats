#ifndef ABSTRACTKITMANAGER_H
#define ABSTRACTKITMANAGER_H

#include <kit/kit.h>

#include <QList>
#include <QRegularExpression>

struct KitsQuery {
    QRegularExpression number;
    QRegularExpression name;
    int type = -1;
};

class AbstractKitManager
{
public:
    virtual ~AbstractKitManager();

    bool hasKitNumbered(const QString &number) const;
    Kit kitNumbered(const QString &number) const;

    bool hasKitNamed(const QString &name) const;
    Kit kitNamed(const QString &name) const;

    QList<Kit> findKits(const KitsQuery &query) const;

    QStringList kitsNames() const;
    QStringList kitsNumbers() const;

    void addKit(const Kit &kit);
    void removeKit(const Kit &kit);

    void forEachKit(const std::function<void(const Kit &kit)> &callback) const;
    void forEachKit(const std::function<void(Kit &kit)> &callback);

protected:
    virtual bool canAddKit(const Kit &kit) const = 0;
    virtual bool canReplaceKit(const Kit &newKit, const Kit &oldKit) const = 0;
    virtual bool canRemoveKit(const Kit &kit) const = 0;

private:
    QList<Kit> m_kits;
};

#endif // ABSTRACTKITMANAGER_H
