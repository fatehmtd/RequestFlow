#pragma once
#include "../view/Node.h"
#include <QTextEdit>
#include "ui_ViewerNodeUi.h"
#include <model/ViewerNode.h>

namespace logic
{
    class ViewerNode : public view::Node
    {
        Q_OBJECT
    public:
        ViewerNode(model::ViewerNode* modelNode);
        virtual void clearUI() override;
    private:
        void setupUi();
        void setJSValue(const QVariant& v);
        void filter(const QString& filter);
    private:
        Ui::ViewerWidgetUiForm _ui;
    };
}