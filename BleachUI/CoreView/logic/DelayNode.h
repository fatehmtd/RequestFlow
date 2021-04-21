#pragma once
#include "../view/Node.h"
#include "ui_DelayNodeUi.h"
#include <model/DelayNode.h>

namespace logic
{
    class DelayNode : public view::Node
    {
    public:
        DelayNode(model::DelayNode* modelNode);

        virtual void clearUI() override;
    private:
        void setupUi();
    private:
        Ui::DelayNodeUiForm _ui;
    };
}
