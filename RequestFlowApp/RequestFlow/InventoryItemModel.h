#pragma once
#include <QAbstractItemModel>
#include <QMimeData>
#include <QList>
#include "InventoryItem.h"

class InventoryItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit InventoryItemModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void setRootItem(InventoryItem* rootItem);
    InventoryItem *getRootItem() const;    

    QStringList mimeTypes() const override;

    QMimeData* mimeData(const QModelIndexList& indexes) const override;

    void setFilter(const QString& filter);
    QString getFilter() const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int columnCount(const QModelIndex &parent) const override;

private:
    bool applyFilter(const InventoryItem* item) const;

    QList<InventoryItem*> filterChildren(const InventoryItem* item) const;

private:
    InventoryItem *getItemFromModelIndex(const QModelIndex& index) const;
    InventoryItem *_rootItem = nullptr;    
    QString _filter;
};
