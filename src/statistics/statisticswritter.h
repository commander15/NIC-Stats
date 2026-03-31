#ifndef STATISTICSWRITTER_H
#define STATISTICSWRITTER_H

#include <common/writer.h>

#include <package/package.h>

#include <statistics/statisticscalculator.h>

#include <QMap>

class StatisticsWritter : public Writer
{
public:
    StatisticsWritter(AbstractKitManager *kitManager = nullptr);
    ~StatisticsWritter();

    void writeStatistics(const Package &package, const QString &fileName, Error *error = nullptr);
    void writeStatistics(const Statistics &stats, const QString &fileName, Error *error = nullptr);

    int writeTableHeader(const Statistics &statistics, QXlsx::Document *document, bool recap = false, int row = 1);
    int writeTableContent(const Statistics &statistics, QXlsx::Document *document, bool recap = false, int startRow = 2);

private:
    StatisticsCalculator::CalculationOptions m_options;
    StatisticsCalculator *m_calculator;
};

#endif // STATISTICSWRITTER_H
