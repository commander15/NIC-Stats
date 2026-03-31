#ifndef ABSTRACTKITMANAGER_H
#define ABSTRACTKITMANAGER_H

#include <kit/kit.h>

#include <QList>
#include <QRegularExpression>

class KitsModel;

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

    bool hasKitMatching(const QRegularExpression &expr) const;

    Kit officeKit() const;
    void setOfficeKit(const Kit &kit);

    QList<Kit> findKits(const KitsQuery &query) const;

    QStringList kitsNames() const;
    QStringList kitsNumbers() const;

    void addKit(const Kit &kit);
    void insertKit(int index, const Kit &kit);
    void removeKit(const Kit &kit);

    void addKits(const QList<Kit> &kits);
    int removeKits(const QList<Kit> &kits);
    int removeKits(const std::function<bool (const Kit &kit)> &pred);

    Kit findKit(const QString &number) const;
    Kit findKit(const QRegularExpression &expr) const;

    void forEachKit(const std::function<void(const Kit &kit)> &callback) const;
    void forEachKit(const std::function<void(Kit &kit)> &callback);

    KitsModel *model() const;

protected:
    virtual bool canAddKit(const Kit &kit) const = 0;
    virtual bool canReplaceKit(const Kit &newKit, const Kit &oldKit) const = 0;
    virtual bool canRemoveKit(const Kit &kit) const = 0;

private:
    int m_officeKitIndex = -1;
    QList<Kit> m_kits;
    KitsModel *m_model = nullptr;
};

#endif // ABSTRACTKITMANAGER_H
