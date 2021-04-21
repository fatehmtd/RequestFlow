#pragma once
#include "../view/Node.h"
#include <QTextEdit>
#include "ui_ViewerNodeUi.h"
#include <model/ViewerNode.h>

namespace logic
{
    class ViewerNode : public view::Node
    {
    public:
        ViewerNode(model::ViewerNode* modelNode);
        virtual void clearUI() override;
    private:
        void setupUi();

    private:
        Ui::ViewerWidgetUiForm _ui;
    };
}