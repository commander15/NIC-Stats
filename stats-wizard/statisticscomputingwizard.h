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
        WelcomePage,
        DispatchPage,
        SetupPage,
        ProcessingPage,
        CompletedPage
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
    void cleanupPage(int id) override;

private slots:
    void processButtonClick(int which);

private:
    void updateButtons(int id);
    void openKitManager();

    Ui::StatisticsComputingWizard *ui;
    AbstractKitManager *m_kitManager;
};

#endif // STATISTICSCOMPUTINGWIZARD_H
