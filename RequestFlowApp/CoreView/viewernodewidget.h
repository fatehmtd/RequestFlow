#pragma once

#include "ui_ViewerNodeUi.h"
#include "coreview_global.h"
#include <QAbstractItemModel>
#include <model/Message.h>
#include <model/ViewerNode.h>

namespace logic
{
    class ViewerNode;

    static constexpr int THRESHOLD = 10000;

    class AbstractItem;

    class COREVIEW_EXPORT CustomModel : public QAbstractItemModel
    {
        Q_OBJECT
    public:
        CustomModel(AbstractItem* rootItem, QObject* parent);

        virtual ~CustomModel();

        QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

        QModelIndex parent(const QModelIndex& child) const override;

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;

        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    private:
        AbstractItem* _root = nullptr;
    };


    class ViewerNodeWidget : public QWidget
    {
        Q_OBJECT
    public:
        ViewerNodeWidget(ViewerNode* node, QWidget* parent);

        void clear();

        void setupUi();

        void setTreeModel(const QVariant& v);
        void filter(const QString& filter);

        void setMessage(model::Message message);
        model::Message getMessage() const;

        void fillData();

        model::ViewerNode *getModelNode() const;
    private:
        Ui::ViewerWidgetUiForm _ui;
        model::Message _message;
        ViewerNode* _node;
    };
}
