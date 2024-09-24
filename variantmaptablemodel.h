#ifndef VARIANTMAPTABLEMODEL_H
#define VARIANTMAPTABLEMODEL_H

#include <QAbstractTableModel>

class AbstractColumn
{
    QString _name;

public:
    AbstractColumn(const QString &name);

    QString name() const { return _name; }

    virtual QVariant columnData(const QVariantMap &rowData, int role = Qt::DisplayRole) = 0;
};

struct SimpleColumn : AbstractColumn
{
    SimpleColumn(const QString &name);

    virtual QVariant columnData(const QVariantMap &rowData, int role = Qt::DisplayRole) override;
};

class VariantMapTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit VariantMapTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void registerColumn(AbstractColumn *column);

    void addRow(const QVariantMap &rowData);

    // Convenience methods
    int columnByName(const QString &name) const;
    QString nameByColumn(int col) const;

private:
    unsigned _rowId = 1;
    QHash<unsigned, QVariantMap> _rowData;
    QList<AbstractColumn*> _columns;
};

#endif // VARIANTMAPTABLEMODEL_H
