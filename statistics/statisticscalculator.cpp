#include "statisticscalculator.h"

#include <kit/abstractkitmanager.h>

#include <enveloppe/enveloppe.h>
#include <enveloppe/record.h>

#include <QRegularExpression>

StatisticsCalculator::StatisticsCalculator(AbstractKitManager *kitManager)
    : m_kitManager(kitManager)
{
}

Statistics StatisticsCalculator::compute(const Package &package) const
{
    Statistics statistics;

    const QStringList kitsNames = m_kitManager->kitsNames();
    for (const QString &kitName : kitsNames) {
        const Kit kit = m_kitManager->kitNamed(kitName);
        statistics.postsStatistics.append(computePostStatistics(kit.name(), kit.regularExpression(), package));
    }

    auto ns = findAllKitsNumbers(package);
    ns.sort();

    int total = statistics.total();
    return statistics;
}

QStringList StatisticsCalculator::findAllKitsNumbers(const Package &package) const
{
    QStringList numbers;

    const QList<Enveloppe> enveloppes = package.enveloppes();
    for (const Enveloppe &enveloppe : enveloppes) {
        numbers.append(enveloppe.recordsKitsNumbers());
        numbers.removeDuplicates();
    }

    return numbers;
}

PostStats StatisticsCalculator::computePostStatistics(const QString &name, const QRegularExpression &kit, const Package &package) const
{
    PostStats stats;
    stats.postName = name;

    const QList<Enveloppe> enveloppes = package.enveloppes();
    for (const Enveloppe &enveloppe : enveloppes) {
        const QList<Record> records = enveloppe.kitRecords(kit);
        stats.statistics.insert(enveloppe.number(), records.size());
    }

    return stats;
}
