#ifndef STATISTICSSETUPPAGE_H
#define STATISTICSSETUPPAGE_H

#include <QWizardPage>
#include <QStringListModel>

namespace Ui {
class StatisticsSetupPage;
}

class StatisticsSetupPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit StatisticsSetupPage(QWidget *parent = nullptr);
    ~StatisticsSetupPage();

    QStringList files() const;

    QString outputDir() const;
    void setOutputDir(const QString &dir);

    bool isComplete() const override;

private slots:
    void addFile();
    void removeFile();
    void changeOutputDir();

private:
    Ui::StatisticsSetupPage *ui;
    QStringListModel model;
    QStringList m_files;
};

#endif // STATISTICSSETUPPAGE_H
