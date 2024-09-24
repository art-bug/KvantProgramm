#include "variantmaptablemodel.h"

VariantMapTableModel::VariantMapTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant VariantMapTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal)
            return _columns.at(section)->name();
    }

    return QVariant();
}

int VariantMapTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _rowData.count();
}

int VariantMapTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _columns.count();
}

QVariant VariantMapTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QVariantMap rowData = _rowData.value(index.row() + 1);
    return _columns.at(index.column())->columnData(rowData, role);
}

void VariantMapTableModel::registerColumn(AbstractColumn *column)
{
    _columns.append(column);
}

void VariantMapTableModel::addRow(const QVariantMap &rowData)
{
    beginInsertRows(QModelIndex(), _rowData.count(), _rowData.count());
    _rowData.insert(_rowId++, rowData);
    endInsertRows();
}

int VariantMapTableModel::columnByName(const QString &name) const
{
    for (int col = 0; col < _columns.count(); col++) {
        if (nameByColumn(col) == name)
            return col;
    }

    return -1;
}

QString VariantMapTableModel::nameByColumn(int col) const
{
    return _columns.at(col)->name();
}

AbstractColumn::AbstractColumn(const QString &name) : _name(name)
{

}

SimpleColumn::SimpleColumn(const QString &name) : AbstractColumn(name)
{

}

QVariant SimpleColumn::columnData(const QVariantMap &rowData, int role)
{
    if (role != Qt::DisplayRole)
        return QVariant();

    return rowData.value(name());
}
