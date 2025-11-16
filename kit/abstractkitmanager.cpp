#include "abstractkitmanager.h"

#include <kit/kitsmodel.h>

AbstractKitManager::~AbstractKitManager()
{
}

bool AbstractKitManager::hasKitNumbered(const QString &number) const
{
    const Kit kit = kitNumbered(number);
    return kit.isValid();
}

Kit AbstractKitManager::kitNumbered(const QString &number) const
{
    if (number.isEmpty()) return Kit();

    auto it = std::find_if(m_kits.begin(), m_kits.end(), [&number](const Kit &kit) {
        const QRegularExpression expr = kit.regularExpression();
        return expr.match(number).hasMatch();
    });

    return (it == m_kits.end() ? Kit() : *it);
}

bool AbstractKitManager::hasKitNamed(const QString &name) const
{
    const Kit kit = kitNamed(name);
    return kit.isValid();
}

Kit AbstractKitManager::kitNamed(const QString &name) const
{
    auto it = std::find_if(m_kits.begin(), m_kits.end(), [&name](const Kit &kit) {
        return kit.name() == name;
    });

    return (it == m_kits.end() ? Kit() : *it);
}

bool AbstractKitManager::hasKitMatching(const QRegularExpression &expr) const
{
    auto it = std::find_if(m_kits.begin(), m_kits.end(), [&expr](const Kit &kit) {
        return kit.regularExpression() == expr;
    });

    return (it == m_kits.end() ? false : true);
}

Kit AbstractKitManager::officeKit() const
{
    if (m_officeKitIndex >= 0 && m_officeKitIndex < m_kits.size())
        return m_kits.at(m_officeKitIndex);
    else
        return Kit();
}

void AbstractKitManager::setOfficeKit(const Kit &kit)
{
    insertKit(0, kit);
    m_officeKitIndex = 0;

    // Notify model
    if (m_model)
        m_model->notifyReset();
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
    insertKit(m_kits.size(), kit);
}

void AbstractKitManager::insertKit(int index, const Kit &kit)
{
    // Existing number, apply replace strategy
    if (hasKitNumbered(kit.number())) {
        const Kit oldKit = kitNumbered(kit.number());
        if (canReplaceKit(kit, oldKit)) {
            const int index = m_kits.indexOf(oldKit);
            m_kits.replace(index, kit);

            // Notify model
            if (m_model)
                m_model->notifyChange(index);
        }
        return;
    }

    // Existing name, apply replace strategy
    if (hasKitNamed(kit.name())) {
        const Kit oldKit = kitNamed(kit.name());
        if (canReplaceKit(kit, oldKit)) {
            const int index = m_kits.indexOf(oldKit);
            m_kits.replace(index, kit);

            // Notify model
            if (m_model)
                m_model->notifyChange(index);
        }
        return;
    }

    // New kit, apply insert strategy
    if (canAddKit(kit)) {
        m_kits.insert(index >= 0 ? index : m_kits.size(), kit);

        // Notify model
        if (m_model)
            m_model->notifyAdd(m_kits.size() - 1);
    }
}

void AbstractKitManager::removeKit(const Kit &kit)
{
    if (m_kits.contains(kit) && canRemoveKit(kit)) {
        m_kits.removeOne(kit);

        // Notify model
        if (m_model)
            m_model->notifyRemove(m_kits.size() - 1);
    }
}

void AbstractKitManager::addKits(const QList<Kit> &kits)
{
    std::for_each(kits.begin(), kits.end(), [this] (const Kit &kit) { addKit(kit); });
}

int AbstractKitManager::removeKits(const QList<Kit> &kits)
{
    return removeKits([kits](const Kit &kit) {
        return kits.contains(kit);
    });
}

int AbstractKitManager::removeKits(const std::function<bool (const Kit &)> &pred)
{
    const int count = m_kits.removeIf(pred);

    // Report model
    if (m_model)
        m_model->notifyReset();

    return count;
}

Kit AbstractKitManager::findKit(const QString &number) const
{
    return findKit(QRegularExpression('^' + number + '$'));
}

Kit AbstractKitManager::findKit(const QRegularExpression &expr) const
{
    auto it = std::find_if(m_kits.begin(), m_kits.end(), [&expr](const Kit &kit) {
        return kit.regularExpression() == expr;
    });

    return (it == m_kits.end() ? Kit() : *it);
}

void AbstractKitManager::forEachKit(const std::function<void (const Kit &)> &callback) const
{
    std::for_each(m_kits.begin(), m_kits.end(), callback);
}

void AbstractKitManager::forEachKit(const std::function<void (Kit &)> &callback)
{
    std::for_each(m_kits.begin(), m_kits.end(), callback);
}

KitsModel *AbstractKitManager::model() const
{
    if (m_model == nullptr) {
        AbstractKitManager *self = const_cast<AbstractKitManager*>(this);
        self->m_model = new KitsModel(self->m_kits, &self->m_officeKitIndex);
    }
    return m_model;
}
