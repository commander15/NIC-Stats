#ifndef KIT_H
#define KIT_H

#include <QSharedDataPointer>

class QJsonObject;

class KitData;
class Kit
{
public:
    enum KitType {
        DesktopKit,
        MobileKit,
        UnknownKit
    };

    Kit();
    Kit(const QString &number, const QString &name, KitType type = UnknownKit);
    Kit(const QRegularExpression &expr, const QString &name, KitType type = UnknownKit);
    Kit(const Kit &);
    Kit(Kit &&);
    Kit &operator=(const Kit &);
    Kit &operator=(Kit &&);
    ~Kit();

    QString number() const;
    void setNumber(const QString &number);

    QString name() const;
    void setName(const QString &name);

    QRegularExpression regularExpression() const;
    void setRegularExpression(const QRegularExpression &expr);

    KitType type() const;
    void setType(KitType type);

    bool isValid() const;

    static Kit fromJsonObject(const QJsonObject &object);
    QJsonObject toJsonObject() const;

    bool operator==(const Kit &other) const;
    inline bool operator!=(const Kit &other) const
    { return !operator==(other); }

private:
    QSharedDataPointer<KitData> data;
};

#endif // KIT_H
