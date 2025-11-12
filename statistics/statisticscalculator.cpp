#include "statisticscalculator.h"

#include <kit/abstractkitmanager.h>

#include <enveloppe/enveloppe.h>
#include <enveloppe/record.h>

#include <QRegularExpression>

StatisticsCalculator::StatisticsCalculator(AbstractKitManager *kitManager)
    : m_kitManager(kitManager)
{
}

Statistics StatisticsCalculator::compute(const Package &package, const CalculationOptions &options) const
{
    Statistics statistics;

    // Date
    statistics.date = package.date();

    // kit numbers extraction
    QStringList kitNumbers;
    if (options.testFlag(IncludeUnknownKits)) {
        kitNumbers = findAllKitsNumbers(package);
        kitNumbers.removeIf([this](const QString &number) {
            const bool found = m_kitManager->hasKitNumbered(number);
            return found;
        });
        kitNumbers.sort();
    }

    // Computing agains known kits
    if (!options.testFlag(ExcludeKownKits)) {
        const QStringList kitsNames = m_kitManager->kitsNames();
        for (const QString &kitName : kitsNames) {
            const Kit kit = m_kitManager->kitNamed(kitName);
            statistics.postsStatistics.append(computePostStatistics(kit.name(), kit.regularExpression(), package));
            kitNumbers.removeOne(kit.number());
        }
    }

    // Computing against unknown kits
    for (const QString &kitNumber : std::as_const(kitNumbers)) {
        const QRegularExpression expr('^' + kitNumber + '$');
        statistics.postsStatistics.append(computePostStatistics(kitNumber, expr, package));
    }

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
