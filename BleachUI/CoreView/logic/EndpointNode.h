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

        virtual void clearUI() override;
    private:
        void initUI();
    private slots:
        void onHttpMethodChanged(int index);
        void onTimeoutChanged(int delay);
        void onContentTypeChanged(const QString& t);
    private:
        Ui::EndpointNodeUiWidget _ui;
    };
}