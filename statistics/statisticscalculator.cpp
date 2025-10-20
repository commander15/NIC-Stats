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

    const QStringList kitsNumbers = m_kitManager->kitsNumbers();
    for (const QString &kitNumber : kitsNumbers) {
        const Kit kit = m_kitManager->kitNumbered(kitNumber);
        statistics.postsStatistics.append(computePostStatistics(kit.name(), kit.regularExpression(), package));
    }

    return statistics;
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
