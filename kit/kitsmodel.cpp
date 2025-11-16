#include "kitsmodel.h"

#include <kit/kit.h>

#include <QRegularExpression>
#include <QColor>

KitsModel::KitsModel(QList<Kit> &kits, int *officeKitIndex)
    : m_kits(kits)
    , m_officeKitIndex(officeKitIndex)
{
}

Kit KitsModel::kit(int row) const
{
    return m_kits.at(row);
}

QVariant KitsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (m_officeKitIndex && section == *m_officeKitIndex && orientation == Qt::Vertical && role == Qt::BackgroundRole) {
        return QColor("purple");
    }

    if (orientation == Qt::Vertical || !valueRoles.contains(role))
        return QAbstractTableModel::headerData(section, orientation, role);

    switch (section) {
    case 0:
        return tr("Number");

    case 1:
        return tr("Post");

    case 2:
        return tr("Type");

    default:
        return QAbstractTableModel::headerData(section, orientation, role);
    }
}

bool KitsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    static int index = 1;
    const int lastRow = row + count - 1;

    beginInsertRows(parent, row, lastRow);
    for (int i(row); i <= lastRow; ++i)
        m_kits.insert(i, Kit("0000", "New Post " + QString::number(index++)));
    endInsertRows();

    return true;
}

bool KitsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    m_kits.remove(row, count);
    endRemoveRows();
    return true;
}

int KitsModel::rowCount(const QModelIndex &parent) const
{
    return (parent.isValid() ? 0 : m_kits.count());
}

int KitsModel::columnCount(const QModelIndex &parent) const
{
    return (parent.isValid() ? 0 : 3);
}

QVariant KitsModel::data(const QModelIndex &index, int role) const
{
    if (m_officeKitIndex && index.row() == *m_officeKitIndex && role == Qt::BackgroundRole) {
        return QColor("purple");
    }

    if (!valueRoles.contains(role))
        return QVariant();

    const Kit kit = m_kits.value(index.row());
    switch (index.column()) {
    case 0: {
        const QString pattern = kit.regularExpression().pattern();
        return pattern.mid(1, pattern.size() - 2);
    }

    case 1:
        return kit.name();

    case 2: {
        switch (kit.type()) {
        case Kit::DesktopKit:
            return tr("DESKTOP");

        case Kit::MobileKit:
            return tr("MOBILE");

        case Kit::UnknownKit:
            return tr("UNKNOWN");
        }
    }

    default:
        return QVariant();
    }
}

bool KitsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!valueRoles.contains(role))
        return QAbstractTableModel::setData(index, value, role);

    Kit kit = m_kits.value(index.row());
    switch (index.column()) {
    case 0:
        kit.setRegularExpression(QRegularExpression('^' + value.toString() + '$'));
        break;

    case 1:
        kit.setName(value.toString());
        break;

    case 2: {
        const QString type = value.toString().trimmed().toUpper();
        if (type == "DESKTOP")
            kit.setType(Kit::DesktopKit);
        else if (type == "MOBILE")
            kit.setType(Kit::MobileKit);
        else
            kit.setType(Kit::UnknownKit);
        break;
    }

    default:
        return false;
    }

    m_kits.replace(index.row(), kit);
    notifyChange(index.row());
    return true;
}

Qt::ItemFlags KitsModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return QAbstractTableModel::flags(index);
}

void KitsModel::notifyChange(int index)
{
    emit dataChanged(this->index(index, 0), this->index(index, columnCount() - 1));
}

void KitsModel::notifyAdd(int index)
{
    beginInsertRows(QModelIndex(), index, index);
    endInsertRows();
}

void KitsModel::notifyRemove(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    endRemoveRows();
}

void KitsModel::notifyReset()
{
    beginResetModel();
    endResetModel();
}
