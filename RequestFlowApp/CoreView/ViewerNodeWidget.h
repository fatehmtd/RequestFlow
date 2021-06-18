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
