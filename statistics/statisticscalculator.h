#ifndef STATISTICSCALCULATOR_H
#define STATISTICSCALCULATOR_H

#include <package/package.h>

#include <QMap>
#include <QDate>

class AbstractKitManager;

struct PostStats {
    QString postName;
    QMap<QString, int> statistics;

    int total() const
    {
        int sum = 0;
        for (int value : statistics)
            sum += value;
        return sum;
    }
};

struct Statistics {
    QDate date;
    QList<PostStats> postsStatistics;

    int total() const
    {
        int sum = 0;
        for (const PostStats &stats : postsStatistics)
            sum += stats.total();
        return sum;
    }
};

class StatisticsCalculator
{
public:
    enum CalculationOption {
        NoOptions = 0x0,
        IncludeUnknownKits = 0x1,
        ExcludeKownKits = 0x2
    };

    Q_DECLARE_FLAGS(CalculationOptions, CalculationOption)

    StatisticsCalculator(AbstractKitManager *kitManager);

    Statistics compute(const Package &package, const CalculationOptions &options) const;
    PostStats computePostStatistics(const QString &name, const QRegularExpression &kit, const Package &package) const;

    QStringList findAllKitsNumbers(const Package &package) const;

private:
    AbstractKitManager *m_kitManager;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(StatisticsCalculator::CalculationOptions)

#endif // STATISTICSCALCULATOR_H
