#ifndef KITSMODEL_H
#define KITSMODEL_H

#include <QAbstractTableModel>
#include <QList>

class Kit;

class KitsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    KitsModel(QList<Kit> &kits, int *officeKitIndex = nullptr);

    Kit kit(int row) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    void notifyChange(int index);
    void notifyAdd(int index);
    void notifyRemove(int index);
    void notifyReset();

    int *m_officeKitIndex;
    QList<Kit> &m_kits;

    static inline QList<int> valueRoles = { Qt::DisplayRole, Qt::EditRole };

    friend class AbstractKitManager;
};

#endif // KITSMODEL_H
