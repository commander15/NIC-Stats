#ifndef STATISTICSPROCESSINGPAGE_H
#define STATISTICSPROCESSINGPAGE_H

#include <QWizardPage>
#include <QTimer>

namespace Ui {
class StatisticsProcessingPage;
}

class Package;

class QThread;

class StatisticsProcessingPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit StatisticsProcessingPage(QWidget *parent = nullptr);
    ~StatisticsProcessingPage();

    void initializePage() override;
    void cleanupPage() override;
    bool isComplete() const override;

    void process(const QString &outputDir, const QStringList &files, class AbstractKitManager *kitManager);

private:
    static Package readPackage(const QStringList &files);
    static void writeEnveloppes(const QList<class Enveloppe> &enveloppes, const QString &dir, AbstractKitManager *kitManager);
    static class Statistics writeStats(const Package &package, const QString &dir, AbstractKitManager *kitManager);

    Ui::StatisticsProcessingPage *ui;
    QTimer m_timer;
    QThread *m_thread;
};

#endif // STATISTICSPROCESSINGPAGE_H
