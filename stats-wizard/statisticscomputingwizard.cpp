#include "statisticscomputingwizard.h"
#include "ui_statisticscomputingwizard.h"

#include <kit/kitmanagerdialog.h>

#include <QDesktopServices>

StatisticsComputingWizard::StatisticsComputingWizard(QWidget *parent)
    : QWizard(parent)
    , ui(new Ui::StatisticsComputingWizard)
    , m_kitManager(nullptr)
{
    ui->setupUi(this);

    ui->versionLabel->setText(QStringLiteral("version %1").arg(APP_VERSION));

    connect(this, &QWizard::customButtonClicked, this, &StatisticsComputingWizard::processButtonClick);
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
    if (id == DispatchPage) {
        const QDate today = QDate::currentDate();
        ui->calendarWidget->setDateRange(today.addDays(-14), today);
        ui->calendarWidget->setSelectedDate(today);

        ui->setupPage->setDateRange(ui->calendarWidget->minimumDate(), ui->calendarWidget->maximumDate());
    }

    if (id == SetupPage) {
        ui->setupPage->setDate(ui->calendarWidget->selectedDate());
    }

    if (id == ProcessingPage) {
        auto setup = ui->setupPage;
        ui->processingPage->process(setup->date(), setup->outputDir(), setup->files(), m_kitManager, setup->generateStats2());
    }

    if (id == CompletedPage) {
        ui->outputDirLabel->setText(ui->setupPage->outputDir());
    }

    updateButtons(id);
    QWizard::initializePage(id);
}

void StatisticsComputingWizard::cleanupPage(int id)
{
    if (id == DispatchPage)
        updateButtons(WelcomePage);

    QWizard::cleanupPage(id);
}

void StatisticsComputingWizard::processButtonClick(int which)
{
    if (which == CustomButton1)
        openKitManager();
}

void StatisticsComputingWizard::updateButtons(int id)
{
    QList<QWizard::WizardButton> buttons;

    // 'Kits' button on welcome
    if (id == WelcomePage)
        buttons << CustomButton1;

    // Navigation buttons on all pages
    buttons << Stretch;
    if (id > WelcomePage && id < ProcessingPage)
        buttons << BackButton;
    if (id < CompletedPage && id < ProcessingPage)
        buttons << NextButton;

    // Cancel or Finish button
    buttons << (id == CompletedPage ? FinishButton : CancelButton);

    setButtonLayout(buttons);

    if (id == WelcomePage) {
        setButtonText(CustomButton1, tr("Kits"));
    }
}

void StatisticsComputingWizard::openKitManager()
{
    KitManagerDialog d(this);
    d.resize(size().shrunkBy(QMargins(32, 32, 32, 32)));
    d.setManager(m_kitManager);
    d.exec();
}
