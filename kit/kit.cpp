#include "kit.h"

#include <QRegularExpression>
#include <QJsonObject>

class KitData : public QSharedData
{
public:
    QString name;
    QRegularExpression regEx;
    Kit::KitType type = Kit::UnknownKit;

    bool equals(const KitData *other) const
    { return name == other->name && regEx == other->regEx && type == other->type; }
};

Kit::Kit()
    : data(new KitData)
{}

Kit::Kit(const QString &number, const QString &name, KitType type)
    : Kit()
{
    setNumber(number);
    setName(name);
    setType(type);
}

Kit::Kit(const QRegularExpression &expr, const QString &name, KitType type)
    : Kit()
{
    setRegularExpression(expr);
    setName(name);
    setType(type);
}

Kit::Kit(const Kit &rhs)
    : data{rhs.data}
{}

Kit::Kit(Kit &&rhs)
    : data{std::move(rhs.data)}
{}

Kit &Kit::operator=(const Kit &rhs)
{
    if (this != &rhs)
        data = rhs.data;
    return *this;
}

Kit &Kit::operator=(Kit &&rhs)
{
    if (this != &rhs)
        data = std::move(rhs.data);
    return *this;
}

Kit::~Kit() {}

QString Kit::number() const
{
    static const QRegularExpression numberExpr(R"(^\d{4}$)");

    const QString pattern = data->regEx.pattern();
    const QString content = pattern.mid(1, pattern.size() - 2);

    if (numberExpr.matchView(content).hasMatch())
        return content;
    else
        return QString();
}

void Kit::setNumber(const QString &number)
{
    data->regEx = QRegularExpression('^' + number + '$');
}

QString Kit::name() const
{
    return data->name;
}

void Kit::setName(const QString &name)
{
    data->name = name;
}

QRegularExpression Kit::regularExpression() const
{
    return data->regEx;
}

void Kit::setRegularExpression(const QRegularExpression &expr)
{
    data->regEx = expr;
}

Kit::KitType Kit::type() const
{
    return data->type;
}

void Kit::setType(KitType type)
{
    data->type = type;
}

bool Kit::isValid() const
{
    return data->regEx.isValid() && !data->name.isEmpty();
}

Kit Kit::fromJsonObject(const QJsonObject &object)
{
    Kit kit;

    // Name
    kit.setName(object.value("name").toString());

    // Expr
    if (object.contains("expr"))
        kit.setRegularExpression(QRegularExpression(object.value("expr").toString()));
    else
        kit.setNumber(object.value("number").toString());

    // Type
    const QString type = object.value("type").toString().toLower();
    if (type == "desktop")
        kit.setType(DesktopKit);
    else if (type == "mobile")
        kit.setType(MobileKit);
    else
        kit.setType(UnknownKit);

    return kit;
}

QJsonObject Kit::toJsonObject() const
{
    QJsonObject object;
    return object;
}

bool Kit::operator==(const Kit &other) const
{
    if (this == &other || data == other.data)
        return true;

    return data->equals(other.data);
}
