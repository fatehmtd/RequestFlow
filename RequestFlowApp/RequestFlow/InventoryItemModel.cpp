#include "InventoryItemModel.h"

InventoryItemModel::InventoryItemModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

int InventoryItemModel::rowCount(const QModelIndex &parent) const
{
    if(_rootItem == nullptr) return 0;

    InventoryItem* currentItem = nullptr;

    if (!parent.isValid())
    {
        currentItem = _rootItem;
    }
    else
    {
        currentItem = getItemFromModelIndex(parent);
    }

    return applyFilter(currentItem) ? filterChildren(currentItem).size() : 0;
}

QVariant InventoryItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto item = getItemFromModelIndex(index);

    switch(role)
    {
    case Qt::DisplayRole:
        return item->getData();
    case Qt::DecorationRole:
        return item->getIcon();
    }

    return QVariant();
}

bool InventoryItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    auto item = getItemFromModelIndex(index);
    if(item == nullptr) return false;

    if(!item->isEditable()) return false;

    if (data(index, role) != value) {
        item->setData(value.toString());\
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags InventoryItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    auto item = getItemFromModelIndex(index);

    if(item == nullptr)
        return Qt::NoItemFlags;

    auto defaultFlags = QAbstractItemModel::flags(index);
    defaultFlags.setFlag(Qt::ItemIsEditable, item->isEditable());
    defaultFlags.setFlag(Qt::ItemIsDragEnabled, item->isDraggable());
    return defaultFlags;
}

void InventoryItemModel::setRootItem(InventoryItem *rootItem)
{
    beginResetModel();
    _rootItem = rootItem;
    endResetModel();
}

InventoryItem *InventoryItemModel::getRootItem() const
{
    return _rootItem;
}

InventoryItem *InventoryItemModel::getItemFromModelIndex(const QModelIndex &index) const
{
    if(!index.isValid()) return nullptr;
    return dynamic_cast<InventoryItem*>(static_cast<QObject*>(index.internalPointer()));
}

QModelIndex InventoryItemModel::index(int row, int column, const QModelIndex &parent) const
{
    InventoryItem* parentItem = parent.isValid() ? getItemFromModelIndex(parent) : _rootItem;
    if(parentItem == nullptr)
    {
        parentItem = _rootItem;
    }
    return createIndex(row, column, filterChildren(parentItem).at(row));
}

QModelIndex InventoryItemModel::parent(const QModelIndex &child) const
{
    if(!child.isValid()) return QModelIndex();
    auto childItem = getItemFromModelIndex(child);
    if(childItem == nullptr) return QModelIndex();
    return createIndex(0, 0, childItem->getParent());
}

int InventoryItemModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

bool InventoryItemModel::applyFilter(const InventoryItem *item) const
{
    auto lowerData = item->getData().toLower();
    auto lowerFilter = _filter.toLower();

    //return lowerData.contains(lowerFilter) || lowerFilter.contains(lowerData);

    if(lowerData.contains(lowerFilter) || lowerFilter.contains(lowerData)) return true;

    for(auto child : item->getChildren())
    {
        if(applyFilter(child)) return true;
    }

    return false;
    //*/
}

QList<InventoryItem *> InventoryItemModel::filterChildren(const InventoryItem *item) const
{
    //return item->getChildren();
    QList<InventoryItem*> filteredItems;
    auto children = item->getChildren();
    std::for_each(children.begin(), children.end(), [=, &filteredItems](InventoryItem* item)
                  {
                      if(applyFilter(item))
                      {
                          filteredItems << item;
                      }
                  });
    return filteredItems;
}

QStringList InventoryItemModel::mimeTypes() const
{
    QStringList output;
    output << "application/x-EndpointEntry";
    return output;
}

#include <model/EndpointEntry.h>
#include <QIODevice>

QMimeData* InventoryItemModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData* mime = new QMimeData();

    QList<model::EndpointEntry*> endpoints;

    std::for_each(indexes.begin(), indexes.end(), [=, &endpoints](const QModelIndex& index)
                  {
                      auto inventoryItem = getItemFromModelIndex(index);
                      if(inventoryItem->getType() == InventoryItem::Type::Endpoint)
                      {
                          auto endpoint = inventoryItem->getUserDataPtr<model::EndpointEntry*>();
                          if(endpoint != nullptr)
                          {
                              endpoints << endpoint;
                          }
                      }
                  });

    if (!endpoints.isEmpty())
    {
        QByteArray array;
        QDataStream outputStream(&array, QIODevice::WriteOnly);
        outputStream << (unsigned int)endpoints.size();

        std::for_each(endpoints.begin(), endpoints.end(), [=, &outputStream](const model::EndpointEntry* endpoint)
                      {
                            outputStream << reinterpret_cast<quint64>(endpoint);
                      });
        mime->setData("application/x-EndpointEntry", array);
    }
    return mime;
}

void InventoryItemModel::setFilter(const QString &filter)
{
    beginResetModel();
    _filter = filter;
    endResetModel();
}

QString InventoryItemModel::getFilter() const
{
    return _filter;
}
