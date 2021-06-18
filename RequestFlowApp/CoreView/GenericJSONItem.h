#pragma once
#include "coreview_global.h"
#include <QVariant>
#include <QColor>
#include <QIcon>
#include <QModelIndex>

namespace logic {
class COREVIEW_EXPORT GenericJSONItem
{
public:
    GenericJSONItem(const QString& path, const QString& value, int type, GenericJSONItem* parent);

    ~GenericJSONItem();

    GenericJSONItem* getParent() const;

    QList<GenericJSONItem*> getChildren() const;

    void setPath(const QString& path);

    QString getPath() const ;

    void setValue(const QString& value);

    QString getValue() const;

    int row() const;

    GenericJSONItem* addChild(const QString& path, const QString& value, int type);

    void setModelIndex(QModelIndex index);

    QModelIndex getModelIndex() const;

    int getNumChildren() const;

    int getType() const;

    QVariant getForeground() const ;

    QVariant getDecoration() const ;

private:
    QString _path, _value;
    int _type;
    GenericJSONItem* _parent = nullptr;
    QList<GenericJSONItem*> _children;

    QVariant _foreground;
    QVariant _decoration;

    QModelIndex _modelIndex;
    int _numChildren = 0;
};

}
