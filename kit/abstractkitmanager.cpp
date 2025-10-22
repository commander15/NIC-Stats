#include "abstractkitmanager.h"

AbstractKitManager::~AbstractKitManager()
{
}

bool AbstractKitManager::hasKitNumbered(const QString &number) const
{
    if (number.isEmpty()) return false;

    auto it = std::find_if(m_kits.begin(), m_kits.end(), [&number](const Kit &kit) {
        const QString currentNumber = kit.number();
        return (currentNumber.isEmpty() ? false : currentNumber == number);
    });

    return it != m_kits.end();
}

Kit AbstractKitManager::kitNumbered(const QString &number) const
{
    if (number.isEmpty()) return Kit();

    auto it = std::find_if(m_kits.begin(), m_kits.end(), [&number](const Kit &kit) {
        const QString currentNumber = kit.number();
        return (currentNumber.isEmpty() ? false : currentNumber == number);
    });

    return (it == m_kits.end() ? Kit() : *it);
}

bool AbstractKitManager::hasKitNamed(const QString &name) const
{
    auto it = std::find_if(m_kits.begin(), m_kits.end(), [&name](const Kit &kit) {
        return kit.name() == name;
    });

    return it != m_kits.end();
}

Kit AbstractKitManager::kitNamed(const QString &name) const
{
    auto it = std::find_if(m_kits.begin(), m_kits.end(), [&name](const Kit &kit) {
        return kit.name() == name;
    });

    return (it == m_kits.end() ? Kit() : *it);
}

Kit AbstractKitManager::officeKit() const
{
    if (!m_officeKitExpr.isValid()) return Kit();

    KitsQuery query;
    query.number = m_officeKitExpr;

    const QList<Kit> kits = findKits(query);
    return (kits.isEmpty() ? Kit() : kits.first());
}

void AbstractKitManager::setOfficeKit(const Kit &kit)
{
    addKit(kit);
    m_officeKitExpr = kit.regularExpression();
}

QList<Kit> AbstractKitManager::findKits(const KitsQuery &query) const
{
    QList<Kit> kits = m_kits;

    kits.removeIf([&query](const Kit &kit) {
        switch (query.type) {
        case Kit::DesktopKit:
        case Kit::MobileKit:
        case Kit::UnknownKit:
            if (query.type != kit.type())
                return true;
        }

        if (query.number.isValid()) {
            if (query.number != kit.regularExpression() && !query.number.matchView(kit.number()).hasMatch())
                return true;
        }

        if (query.name.isValid()) {
            if (!query.name.matchView(kit.name()).hasMatch())
                return true;
        }

        return false;
    });

    return kits;
}

QStringList AbstractKitManager::kitsNames() const
{
    QStringList names(m_kits.size());
    std::transform(m_kits.begin(), m_kits.end(), names.begin(), [](const Kit &kit) {
        return kit.name();
    });
    return names;
}

QStringList AbstractKitManager::kitsNumbers() const
{
    QStringList numbers(m_kits.size());
    std::transform(m_kits.begin(), m_kits.end(), numbers.begin(), [](const Kit &kit) {
        return kit.number();
    });
    return numbers;
}

void AbstractKitManager::addKit(const Kit &kit)
{
    // Existing number, apply replace strategy
    if (hasKitNumbered(kit.number())) {
        const Kit oldKit = kitNumbered(kit.number());
        if (canReplaceKit(kit, oldKit))
            m_kits.replace(m_kits.indexOf(oldKit), kit);
        return;
    }

    // Existing name, apply replace strategy
    if (hasKitNamed(kit.name())) {
        const Kit oldKit = kitNamed(kit.name());
        if (canReplaceKit(kit, oldKit))
            m_kits.replace(m_kits.indexOf(oldKit), kit);
        return;
    }

    // New kit, apply add strategy
    if (canAddKit(kit)) {
        m_kits.append(kit);
    }
}

void AbstractKitManager::removeKit(const Kit &kit)
{
    if (canRemoveKit(kit)) {
        m_kits.removeOne(kit);
    }
}

void AbstractKitManager::forEachKit(const std::function<void (const Kit &)> &callback) const
{
    std::for_each(m_kits.begin(), m_kits.end(), callback);
}

void AbstractKitManager::forEachKit(const std::function<void (Kit &)> &callback)
{
    std::for_each(m_kits.begin(), m_kits.end(), callback);
}
