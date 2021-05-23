#include "inventoryitem.h"

InventoryItem::InventoryItem(int type, InventoryItem *parent) : QObject(parent), _type(type)
{

}

InventoryItem *InventoryItem::getParent() const
{
    return dynamic_cast<InventoryItem*>(parent());
}

QList<InventoryItem *> InventoryItem::getChildren() const
{
    return findChildren<InventoryItem*>("", Qt::FindChildOption::FindDirectChildrenOnly);
}

void InventoryItem::setData(const QString &data)
{
    _data = data;
}

QString InventoryItem::getData() const
{
    return _data;
}

int InventoryItem::getType() const
{
    return _type;
}

int InventoryItem::isDraggable() const
{
    switch(_type)
    {
    case Type::Node:
    case Type::Endpoint:
        return true;
    }
    return false;
}

bool InventoryItem::isEditable() const
{
    return _type == Type::Node;
}

void InventoryItem::setIcon(QIcon icon)
{
    _icon = icon;
}

QIcon InventoryItem::getIcon() const
{
    return _icon;
}

void InventoryItem::setUserDataPtr(void *ptr)
{
    _dataPtr = ptr;
}

void *InventoryItem::getUserDataPtr() const
{
    return _dataPtr;
}
