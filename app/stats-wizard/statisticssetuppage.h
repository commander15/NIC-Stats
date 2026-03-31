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

    QDate date() const;
    void setDate(const QDate &date);
    void setDateRange(const QDate &min, const QDate &max);

    QStringList files() const;

    QString outputDir() const;
    void setOutputDir(const QString &dir);

    bool mustCreateSubDir() const;
    bool generateStats2() const;

    bool isComplete() const override;

private slots:
    void addFile();
    void addFilesFromDir();
    void removeFile();
    void changeOutputDir();

private:
    void addSomeFiles(const QStringList &fileNames, const QString &dir = QString());
    void removeSomeFiles(const QList<int> &indexes);

    Ui::StatisticsSetupPage *ui;
    QStringListModel model;
    QStringList m_files;
};

#endif // STATISTICSSETUPPAGE_H
