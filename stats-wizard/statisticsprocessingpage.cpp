#include "statisticsprocessingpage.h"
#include "ui_statisticsprocessingpage.h"

#include <enveloppe/enveloppereader.h>
#include <enveloppe/enveloppewriter.h>
#include <statistics/statisticswritter.h>

#include <QAbstractButton>
#include <QThread>

StatisticsProcessingPage::StatisticsProcessingPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::StatisticsProcessingPage)
{
    ui->setupUi(this);

    m_timer.setInterval(3000);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, [this] {
        emit completeChanged();
        QTimer::singleShot(100, wizard(), &QWizard::next);
    });

    setCommitPage(true);
}

StatisticsProcessingPage::~StatisticsProcessingPage()
{
    delete ui;
}

void StatisticsProcessingPage::initializePage()
{
    QTimer::singleShot(0, this, [this] {
        QWizard *wizard = this->wizard();
        wizard->button(QWizard::BackButton)->setEnabled(false);
        wizard->button(QWizard::CommitButton)->setText(tr("Next"));
    });
}

void StatisticsProcessingPage::cleanupPage()
{
    if (m_timer.isActive())
        m_timer.stop();
}

bool StatisticsProcessingPage::isComplete() const
{
    return m_thread == nullptr && !m_timer.isActive();
}

void StatisticsProcessingPage::process(const QString &outputDir, const QStringList &files, AbstractKitManager *kitManager)
{
    m_thread = QThread::create([outputDir, files, kitManager] {
        const Package package = readPackage(files);
        writeEnveloppes(package.enveloppes(), outputDir, kitManager);
        writeStats(package, outputDir, kitManager);
    });

    connect(m_thread, &QThread::finished, &m_timer, QOverload<>::of(&QTimer::start));
    connect(m_thread, &QThread::finished, this, [this] {
        m_thread = nullptr;
        m_timer.start();
    });

    m_thread->start();
}

Package StatisticsProcessingPage::readPackage(const QStringList &files)
{
    EnveloppeReader reader;

    Package package;
    for (const QString &file : files) {
        const Enveloppe enveloppe = reader.readEnveloppe(file);
        package.addEnveloppe(enveloppe);
    }
    return package;
}

void StatisticsProcessingPage::writeEnveloppes(const QList<Enveloppe> &enveloppes, const QString &dir, AbstractKitManager *kitManager)
{
    EnveloppeWriter writer(kitManager);
    for (const Enveloppe &enveloppe : enveloppes)     {
        writer.writeEnveloppe(enveloppe, dir + '/' + enveloppe.number() + ".xlsx");
    }
}

Statistics StatisticsProcessingPage::writeStats(const Package &package, const QString &dir, AbstractKitManager *kitManager)
{
    StatisticsCalculator calculator(kitManager);
    const Statistics stats = calculator.compute(package);

    StatisticsWritter writer;
    writer.writeStatistics(stats, dir + "/STATISTIQUES.xlsx");

    return stats;
}
