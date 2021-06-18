#include "GenericJSONItem.h"

#include "CoreViewUtils.h"

logic::GenericJSONItem::GenericJSONItem(const QString& path, const QString& value, int type, GenericJSONItem* parent)
    : _path(path), _value(value), _type(type), _parent(parent)
{
    switch (type)
    {
    case QVariant::Type::Map:
    {
        _decoration = view::SVGRenderer::getInstance()->renderToIcon(":/ui/{}");
        _foreground = QColor(0, 0, 255);
        break;
    }
    case QVariant::Type::List:
    {
        _decoration = view::SVGRenderer::getInstance()->renderToIcon(":/ui/[]");
        _foreground = QColor(0, 0, 255);
        break;
    }
    case QVariant::Type::String:
    {
        _foreground = QColor("#AC0D8A");
        break;
    }
    case QVariant::Type::Bool:
    {
        _foreground = QColor("#002CF5");
        break;
    }
    case QVariant::Type::Int:
    case QVariant::Type::UInt:
    case QVariant::Type::LongLong:
    case QVariant::Type::ULongLong:
    case QVariant::Type::Double:
    {
        _foreground = QColor("#00875C");
        break;
    }
    }
}

logic::GenericJSONItem::~GenericJSONItem()
{
    qDeleteAll(_children);
}

logic::GenericJSONItem* logic::GenericJSONItem::getParent() const
{
    return _parent;
}

QList<logic::GenericJSONItem*> logic::GenericJSONItem::getChildren() const
{
    return _children;
}

void logic::GenericJSONItem::setPath(const QString& path)
{
    _path = path;
}

QString logic::GenericJSONItem::getPath() const { return _path; }

void logic::GenericJSONItem::setValue(const QString& value)
{
    _value = value;
}

QString logic::GenericJSONItem::getValue() const { return _value; }

int logic::GenericJSONItem::row() const
{
    if (_parent != nullptr)
    {
        return _parent->_children.indexOf(const_cast<GenericJSONItem*>(this));
    }
    return 0;
}

logic::GenericJSONItem* logic::GenericJSONItem::addChild(const QString& path, const QString& value, int type)
{
    auto item = new GenericJSONItem(path, value, type, this);
    _children << item;
    ++_numChildren;
    return item;
}

void logic::GenericJSONItem::setModelIndex(QModelIndex index)
{
    _modelIndex = index;
}

QModelIndex logic::GenericJSONItem::getModelIndex() const
{
    return _modelIndex;
}

int logic::GenericJSONItem::getNumChildren() const
{
    return _numChildren;
}

int logic::GenericJSONItem::getType() const
{
    return _type;
}

QVariant logic::GenericJSONItem::getForeground() const {
    return _foreground;
}

QVariant logic::GenericJSONItem::getDecoration() const {
    return _decoration;
}
