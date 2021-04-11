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

        virtual QJSValue toJSValue(QJSEngine& engine) const;
        virtual void fromJSValue(const QJSValue& jsValue);

    private:
        void setupUi();
    private:
        Ui::DelayNodeUiForm _ui;
    };
}
