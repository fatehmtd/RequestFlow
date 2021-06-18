#pragma once

#include "coreview_global.h"
#include <QAbstractItemModel>
#include <model/Message.h>
#include <model/ViewerNode.h>
#include <QIcon>
#include <QColor>

#include "GenericJSONItem.h"

namespace logic
{
class COREVIEW_EXPORT CustomJSONModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    CustomJSONModel(GenericJSONItem* rootItem, QObject* parent);

    virtual ~CustomJSONModel();

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex& child) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
private:
    GenericJSONItem* _root = nullptr;
};

}
