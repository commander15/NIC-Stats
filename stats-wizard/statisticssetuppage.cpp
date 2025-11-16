#include "statisticssetuppage.h"
#include "ui_statisticssetuppage.h"

#include <QMenu>
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

    QMenu *addMenu = new QMenu(ui->addFileButton);
    QAction *addFileAction = addMenu->addAction("Add files");
    connect(addFileAction, &QAction::triggered, this, &StatisticsSetupPage::addFile);
    QAction *addFilesFromDirAction = addMenu->addAction("Add file(s) from directory");
    connect(addFilesFromDirAction, &QAction::triggered, this, &StatisticsSetupPage::addFilesFromDir);

    ui->addFileButton->setMenu(addMenu);
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

void StatisticsSetupPage::setDate(const QDate &date)
{
    ui->dateEdit->setDate(date);
}

void StatisticsSetupPage::setDateRange(const QDate &min, const QDate &max)
{
    ui->dateEdit->setDateRange(min, max);
}

QStringList StatisticsSetupPage::files() const
{
    return m_files;
}

QString StatisticsSetupPage::outputDir() const
{
    QString dir = QDir::fromNativeSeparators(ui->outputDirOutput->text());
    if (ui->createSubDirInput->isChecked())
        dir.append("/STATS " + ui->dateEdit->date().toString("dd MMM yyyy").toUpper());
    return dir;
}

void StatisticsSetupPage::setOutputDir(const QString &dir)
{
    ui->outputDirOutput->setText(QDir::toNativeSeparators(dir));
}

bool StatisticsSetupPage::mustCreateSubDir() const
{
    return ui->createSubDirInput->isChecked();
}

bool StatisticsSetupPage::generateStats2() const
{
    return ui->genAdditionalStatsInput->isChecked();
}

bool StatisticsSetupPage::isComplete() const
{
    return model.rowCount() > 0;
}

void StatisticsSetupPage::addFile()
{
    const QStringList newFiles = QFileDialog::getOpenFileNames(this, tr("Select file"), QString(), tr("Excel files") + "(*.xlsx)");
    if (newFiles.isEmpty()) return;

    addSomeFiles(newFiles);
}

void StatisticsSetupPage::addFilesFromDir()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Select directory"));
    if (path.isEmpty())
        return;

    QDir dir(path);
    dir.refresh();

    const QStringList files = dir.entryList({ "*.xlsx" }, QDir::Files, QDir::Name);
    addSomeFiles(files, dir.absolutePath());
}

void StatisticsSetupPage::removeFile()
{
    const QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    QList<int> rows;
    std::transform(indexes.begin(), indexes.end(), std::back_inserter(rows), [](const QModelIndex &index) {
        return index.row();
    });
    removeSomeFiles(rows);
}

void StatisticsSetupPage::changeOutputDir()
{
    const QString currentDir = QDir::fromNativeSeparators(ui->outputDirOutput->text());
    const QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"), currentDir);
    if (dir.isEmpty()) return;

    ui->outputDirOutput->setText(QDir::toNativeSeparators(dir));
}

void StatisticsSetupPage::addSomeFiles(const QStringList &fileNames, const QString &dir)
{
    if (dir.isEmpty()) {
        m_files.append(fileNames);
    } else {
        for (const QString &fileName : fileNames)
            m_files.append(dir + '/' + fileName);
    }

    // Removing duplicates
    m_files.removeDuplicates();

    // Updating model data
    QStringList displayNames;
    std::transform(m_files.begin(), m_files.end(), std::back_inserter(displayNames), [](const QString &fileName) {
        return fileName.section('/', -1).section('.', 0, -2);
    });
    model.setStringList(displayNames);

    for (int i(0); i < m_files.size(); ++i)
        model.setData(model.index(i), QDir::toNativeSeparators(m_files.at(i)), Qt::ToolTipRole);
}

void StatisticsSetupPage::removeSomeFiles(const QList<int> &indexes)
{
    for (int index : indexes)
        m_files.removeAt(index);
    addSomeFiles({});
}
