#include "statisticscomputingwizard.h"
#include "ui_statisticscomputingwizard.h"

#include <QDesktopServices>

StatisticsComputingWizard::StatisticsComputingWizard(QWidget *parent)
    : QWizard(parent)
    , ui(new Ui::StatisticsComputingWizard)
    , m_kitManager(nullptr)
{
    ui->setupUi(this);
}

StatisticsComputingWizard::~StatisticsComputingWizard()
{
    delete ui;
}

void StatisticsComputingWizard::setKitManager(AbstractKitManager *manager)
{
    m_kitManager = manager;
}

void StatisticsComputingWizard::done(int r)
{
    if (r == QDialog::Accepted && ui->openOutputDirOnCloseInput->isChecked()) {
        const QString dir = ui->setupPage->outputDir();
        QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
    }

    QDialog::done(r);
}

void StatisticsComputingWizard::initializePage(int id)
{
    if (id == ProcessingPage) {
        auto setup = ui->setupPage;
        ui->processingPage->process(setup->outputDir(), setup->files(), m_kitManager);
    }

    if (id == CompletedPage) {
        ui->outputDirLabel->setText(ui->setupPage->outputDir());
    }

    QWizard::initializePage(id);
}
