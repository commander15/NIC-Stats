#include "statisticssetuppage.h"
#include "ui_statisticssetuppage.h"

#include <QStandardPaths>
#include <QFileDialog>

StatisticsSetupPage::StatisticsSetupPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::StatisticsSetupPage)
{
    ui->setupUi(this);

    const QDate today = QDate::currentDate();
    ui->dateEdit->setDateRange(today.addDays(-14), today);
    ui->dateEdit->setDate(today);

    ui->listView->setModel(&model);

    connect(ui->addFileButton, &QAbstractButton::clicked, this, &StatisticsSetupPage::addFile);
    connect(ui->removeFileButton, &QAbstractButton::clicked, this, &StatisticsSetupPage::removeFile);
    connect(ui->changeOutputDirButtonButton, &QAbstractButton::clicked, this, &StatisticsSetupPage::changeOutputDir);

    connect(&model, &QAbstractItemModel::rowsInserted, this, [this](const QModelIndex &, int, int) {
        emit completeChanged();
    });

    connect(&model, &QAbstractItemModel::rowsMoved, this, [this](const QModelIndex &, int, int) {
        emit completeChanged();
    });

    connect(&model, &QAbstractItemModel::modelReset, this, &QWizardPage::completeChanged);

    setOutputDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
}

StatisticsSetupPage::~StatisticsSetupPage()
{
    delete ui;
}

QDate StatisticsSetupPage::date() const
{
    return ui->dateEdit->date();
}

QStringList StatisticsSetupPage::files() const
{
    return m_files;
}

QString StatisticsSetupPage::outputDir() const
{
    return QDir::fromNativeSeparators(ui->outputDirOutput->text());
}

void StatisticsSetupPage::setOutputDir(const QString &dir)
{
    ui->outputDirOutput->setText(QDir::toNativeSeparators(dir));
}

bool StatisticsSetupPage::isComplete() const
{
    return model.rowCount() > 0;
}

void StatisticsSetupPage::addFile()
{
    const QStringList newFiles = QFileDialog::getOpenFileNames(this, tr("Select file"), QString(), tr("Excel files") + "(*.xlsx)");
    if (newFiles.isEmpty()) return;

    m_files.append(newFiles);
    m_files.removeDuplicates();

    QStringList files(m_files.size());
    std::transform(m_files.cbegin(), m_files.cend(), files.begin(), [](const QString &fileName) {
        return fileName.section("/", -1);
    });
    model.setStringList(files);
}

void StatisticsSetupPage::removeFile()
{
    const int row = ui->listView->currentIndex().row();
    model.removeRows(row, 1);
}

void StatisticsSetupPage::changeOutputDir()
{
    const QString currentDir = QDir::fromNativeSeparators(ui->outputDirOutput->text());
    const QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"), currentDir);
    if (dir.isEmpty()) return;

    ui->outputDirOutput->setText(QDir::toNativeSeparators(dir));
}
