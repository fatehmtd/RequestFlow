#pragma once
#include "../coremodel_global.h"
#include "Node.h"
#include "Message.h"
#include "Graph.h"
#include "Slot.h"

namespace model
{
    class COREMODEL_EXPORT PayloadNode : public model::Node
    {
        Q_OBJECT
    public:
        PayloadNode(model::Graph* graph);
        void createModel() override;

        void setMessage(const model::Message& message);
        model::Message getMessage() const;

        void evaluate() override;
        OutputSlot* getOutputSlot() const;
    private:
        model::Message _message;
        OutputSlot* _outputSlot = nullptr;
    };
}