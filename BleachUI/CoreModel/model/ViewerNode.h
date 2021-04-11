#pragma once
#include "../coremodel_global.h"
#include "Node.h"
#include "Message.h"
#include "Graph.h"
#include "Slot.h"

namespace model
{
    class Graph;

    class COREMODEL_EXPORT ViewerNode : public model::Node
    {
    public:
        ViewerNode(model::Graph* graph);

        void createModel() override;

        void evaluate() override;

        model::InputSlot* getInput() const;
    private:
        model::InputSlot* _inputSlot = nullptr;
    };
}