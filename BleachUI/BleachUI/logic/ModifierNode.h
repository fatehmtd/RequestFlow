#pragma once
#include "../view/Node.h"
#include "ui_ModifierNodeUi.h"

namespace logic
{
    class ModifierNode : public view::Node
    {
    public:
        ModifierNode(model::Node* modelNode);

        virtual void clearUI() override;
    private:
        void setupUi();
    private:
        Ui::ModifierNodeUiForm* _ui = nullptr;
    };
}

