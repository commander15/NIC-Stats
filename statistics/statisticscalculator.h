#ifndef STATISTICSCALCULATOR_H
#define STATISTICSCALCULATOR_H

#include <package/package.h>

#include <QMap>

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
    StatisticsCalculator(AbstractKitManager *kitManager);

    Statistics compute(const Package &package) const;

    PostStats computePostStatistics(const QString &name, const QRegularExpression &kit, const Package &package) const;

private:
    AbstractKitManager *m_kitManager;
};

#endif // STATISTICSCALCULATOR_H
