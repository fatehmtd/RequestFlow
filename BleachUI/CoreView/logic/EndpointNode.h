#pragma once
#include "../view/Node.h"
#include "../view/Slot.h"
#include <model/EndpointNode.h>
#include "ui_EndpointNodeUi.h"

namespace logic
{
    class EndpointNode : public view::Node
    {
        Q_OBJECT
    public:
        Q_INVOKABLE EndpointNode(model::EndpointNode* modelNode);

        virtual QJSValue toJSValue(QJSEngine& engine) const override;
        virtual void fromJSValue(const QJSValue& jsValue) override;

        virtual void clearUI() override;
    private:
        void initUI();
    private:
        Ui::EndpointNodeUiWidget _ui;
    };
}