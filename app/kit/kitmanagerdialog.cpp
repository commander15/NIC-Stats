#include "kitmanagerdialog.h"
#include "ui_kitmanagerdialog.h"

#include <kit/abstractkitmanager.h>
#include <kit/kitsmodel.h>

#include <QLineEdit>
#include <QComboBox>
#include <QDate>
#include <QTimer>

KitManagerDialog::KitManagerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::KitManagerDialog)
    , m_manager(nullptr)
{
    ui->setupUi(this);

    ui->tableView->setItemDelegate(new KitItemDelegate(this));

    connect(ui->addButton, &QAbstractButton::clicked, this, &KitManagerDialog::addKit);
    connect(ui->deleteButton, &QAbstractButton::clicked, this, &KitManagerDialog::deleteSelectedKits);
}

KitManagerDialog::~KitManagerDialog()
{
    delete ui;
}

void KitManagerDialog::setManager(AbstractKitManager *manager)
{
    if (manager == nullptr) {
        ui->tableView->setModel(nullptr);
    } else {
        ui->tableView->setModel(manager->model());

        QHeaderView *header = ui->tableView->horizontalHeader();
        header->setSectionResizeMode(1, QHeaderView::Stretch);

        QItemSelectionModel *selection = ui->tableView->selectionModel();
        connect(selection, &QItemSelectionModel::selectionChanged, this, &KitManagerDialog::processSelectionChange);
    }

    ui->addButton->setEnabled(manager != nullptr);
    ui->deleteButton->setEnabled(false);

    m_manager = manager;
}

int KitManagerDialog::exec()
{
    ui->tableView->setCurrentIndex(QModelIndex());
    return QDialog::exec();
}

void KitManagerDialog::processSelectionChange(const QItemSelection &selected, const QItemSelection &deselected)
{
    ui->deleteButton->setEnabled(!selected.isEmpty());
}

void KitManagerDialog::addKit()
{
    static int index = 0;

    QString newIndex = QString::number(index++);
    while (newIndex.length() < 4) newIndex.prepend('0');
    m_manager->addKit(Kit(newIndex, "New Post " + QString::number(index)));

    QTimer::singleShot(0, this, [this] {
        auto *model = ui->tableView->model();
        ui->tableView->scrollToBottom();
        ui->tableView->edit(model->index(model->rowCount() - 1, 0));
    });
}

void KitManagerDialog::deleteSelectedKits()
{
    const QModelIndexList indexes = ui->tableView->selectionModel()->selectedRows();

    m_manager->removeKits([&indexes](const Kit &kit) {
        QString pattern = kit.regularExpression().pattern();
        pattern = pattern.mid(1, pattern.size() - 2);

        auto it = std::find_if(indexes.begin(), indexes.end(), [&pattern](const QModelIndex &index) {
            return index.data().toString() == pattern;
        });

        return (it != indexes.end());
    });
}

QWidget *KitItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 0) {
        const int maxPostCount = 10;
        QLineEdit *edit = new QLineEdit(parent);
        edit->setPlaceholderText(tr("eg: 0423"));
        edit->setMaxLength((maxPostCount * 10) + (maxPostCount - 1));
        edit->setValidator(new QRegularExpressionValidator(QRegularExpression(R"(^([0-9.|]{4,5})*$)"), edit));
        //edit->setValidator(new QRegularExpressionValidator(QRegularExpression(R"(^\d{4}|\d{1}\.{3}|\d{2}\.{2}|\d{3}\.{1}$)"), edit));
        return edit;
    }

    if (index.column() == 2) {
        QComboBox *box = new QComboBox(parent);
        box->addItems({ "DESKTOP", "MOBILE" });
        return box;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}
