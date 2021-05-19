#pragma once
#include "../view/Node.h"
#include <model/AssertionNode.h>
#include "ui_AssertNodeUi.h"

namespace logic
{
    class COREVIEW_EXPORT AssertionNode : public view::Node
    {
        Q_OBJECT
    public:
        Q_INVOKABLE AssertionNode(model::AssertionNode* modelNode);

        virtual void clearUI() override;

    private:
        void setupUi();
    private:        
        Ui::AssertNodeWidgetUi _ui;
    };
}
