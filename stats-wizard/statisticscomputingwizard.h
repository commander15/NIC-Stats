#ifndef STATISTICSCOMPUTINGWIZARD_H
#define STATISTICSCOMPUTINGWIZARD_H

#include <package/package.h>
#include <statistics/statisticscalculator.h>

#include <QWizard>

namespace Ui {
class StatisticsComputingWizard;
}

class AbstractKitManager;

class StatisticsComputingWizard : public QWizard
{
    Q_OBJECT

public:
    enum Page {
        WelcomePage = 0,
        SetupPage = 1,
        ProcessingPage = 2,
        CompletedPage = 3
    };

    explicit StatisticsComputingWizard(QWidget *parent = nullptr);
    ~StatisticsComputingWizard();

    Package package() const;
    Statistics statistics() const;

    void setKitManager(AbstractKitManager *manager);

public slots:
    void done(int r) override;

protected:
    void initializePage(int id) override;

private:
    Ui::StatisticsComputingWizard *ui;
    AbstractKitManager *m_kitManager;
};

#endif // STATISTICSCOMPUTINGWIZARD_H
