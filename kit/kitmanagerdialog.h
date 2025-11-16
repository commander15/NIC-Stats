#ifndef KITMANAGERDIALOG_H
#define KITMANAGERDIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>

namespace Ui {
class KitManagerDialog;
}

class AbstractKitManager;

class QItemSelection;

class KitManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KitManagerDialog(QWidget *parent = nullptr);
    ~KitManagerDialog();

    void setManager(AbstractKitManager *manager);

    int exec() override;

private slots:
    void processSelectionChange(const QItemSelection &selected, const QItemSelection &deselected);
    void addKit();
    void deleteSelectedKits();

private:
    Ui::KitManagerDialog *ui;
    AbstractKitManager *m_manager;
};

class KitItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    KitItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // KITMANAGERDIALOG_H
