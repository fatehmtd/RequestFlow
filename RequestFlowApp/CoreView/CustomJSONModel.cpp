#include "CustomJSONModel.h"

logic::CustomJSONModel::CustomJSONModel(GenericJSONItem* rootItem, QObject* parent) : _root(rootItem), QAbstractItemModel(parent)
{

}

logic::CustomJSONModel::~CustomJSONModel()
{
    if (_root != nullptr)
        delete _root;
}

QModelIndex logic::CustomJSONModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    GenericJSONItem* parentItem = nullptr;

    if (!parent.isValid())
        parentItem = _root;
    else
        parentItem = static_cast<GenericJSONItem*>(parent.internalPointer());

    auto item = parentItem->getChildren()[row];

    auto modelIndex = createIndex(row, column, item);
    return modelIndex;
}

QModelIndex logic::CustomJSONModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return QModelIndex();

    auto childItem = static_cast<GenericJSONItem*>(child.internalPointer());

    auto parentItem = childItem->getParent();

    if (parentItem == nullptr)
        return QModelIndex();

    auto modelIndex = createIndex(parentItem->row(), 0, parentItem);
    return modelIndex;
}

int logic::CustomJSONModel::rowCount(const QModelIndex& parent) const
{
    GenericJSONItem* parentItem = nullptr;

    if (!parent.isValid())
        parentItem = _root;
    else
        parentItem = static_cast<GenericJSONItem*>(parent.internalPointer());

    return parentItem->getNumChildren();
}

int logic::CustomJSONModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant logic::CustomJSONModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (section == 0) return "Key";
        else return "Value";
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

QVariant logic::CustomJSONModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto item = static_cast<GenericJSONItem*>(index.internalPointer());

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
    {
        switch (index.column())
        {
        case 0:
            return QString("%1 : %2").arg(item->getPath()).arg(item->getValue());
        //case 1:
        //return item->getValue();
        default:
            return QVariant();
        }
    }
    case Qt::ForegroundRole:
    {
        return item->getForeground();
    }
    case Qt::DecorationRole:
    {
        return item->getDecoration();
    }
    }
    return QVariant();
}
