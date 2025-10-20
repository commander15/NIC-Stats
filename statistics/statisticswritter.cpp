#include "statisticswritter.h"

#include <enveloppe/enveloppe.h>
#include <enveloppe/record.h>

#include <QRegularExpression>

#include <xlsxdocument.h>
#include <xlsxcellformula.h>

StatisticsWritter::StatisticsWritter(AbstractKitManager *kitManager)
    : m_calculator(kitManager == nullptr ? nullptr : new StatisticsCalculator(kitManager))
{
}

StatisticsWritter::~StatisticsWritter()
{
    if (m_calculator != nullptr) {
        delete m_calculator;
    }
}

void StatisticsWritter::writeStatistics(const Package &package, const QString &fileName, Error *error)
{
    // We can't compute without a calculator
    if (m_calculator == nullptr) return;

    const Statistics computed = m_calculator->compute(package);
    writeStatistics(computed, fileName, error);
}

void StatisticsWritter::writeStatistics(const Statistics &statistics, const QString &fileName, Error *error)
{
    QXlsx::Document *document = openFile(fileName, QIODevice::WriteOnly, error);
    if (document == nullptr) return;

    // Clearing
    deleteAllSheets(document);

    // Renaming sheet
    document->renameSheet(document->currentSheet()->sheetName(), "Statistiques");

    // Writing headers
    writeTableHeader(statistics, document);

    // Writing content
    writeTableContent(statistics, document);

    // Saving
    closeFile(document);
}

int StatisticsWritter::writeTableHeader(const Statistics &statistics, QXlsx::Document *document, int row)
{
    QXlsx::Format format;
    format.setFontSize(14);
    format.setFontBold(true);

    document->write(row, 1, "Poste", format);
    document->setColumnWidth(1, 40);

    if (statistics.postsStatistics.isEmpty())
        return row;

    int i = 2;
    const PostStats stats = statistics.postsStatistics.constFirst();
    const QStringList enveloppes = stats.statistics.keys();
    for (const QString &enveloppe : enveloppes) {
        // Writing enveloppe number
        document->write(row, i, enveloppe, format);

        // Resizing column
        document->setColumnWidth(i, 20);

        ++i;
    }

    document->write(row, i, "TOTAL", format);
    document->setColumnWidth(i, 12);

    return row;
}

int StatisticsWritter::writeTableContent(const Statistics &statistics, QXlsx::Document *document, int startRow)
{
    int row = startRow;
    int column = 1;

    // Post formating
    QXlsx::Format postFormat;
    postFormat.setFontSize(14);
    postFormat.setFontBold(true);

    // Will hold enveloppes for total row generation
    QStringList allEnveloppes;

    // Adding stats
    for (const PostStats &stats : statistics.postsStatistics) {
        // Adding post name
        document->write(row, 1, stats.postName, postFormat);

        // Adding count per enveloppe
        const QStringList enveloppes = stats.statistics.keys();
        for (int i(0); i < enveloppes.size(); ++i)
            document->write(row, i + 2, stats.statistics.value(enveloppes.at(i)));

        // Fill enveloppes for total computations
        if (allEnveloppes.isEmpty())
            allEnveloppes = enveloppes;

        // Adding total cell
        const QXlsx::CellRange range(row, 2, row, 1 + enveloppes.size());
        document->write(row, enveloppes.size() + 2, "=SUM(" + range.toString() + ')');

        // Going to next row
        ++row;
    }

    // Adding total row
    const int postsCount = statistics.postsStatistics.size();
    document->write(row, 1, "TOTAL", postFormat);
    for (int i(0); i <= allEnveloppes.count(); ++i) {
        const int column = i + 2;
        const QXlsx::CellRange range(2, i + 2, 1 + postsCount, i + 2);
        document->write(row, column, "=SUM(" + range.toString() + ')');
    }

    return row;
}
