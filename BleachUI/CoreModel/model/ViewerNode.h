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
        Q_OBJECT
    public:
        Q_INVOKABLE ViewerNode(model::Graph* graph);

        void createModel() override;

        void evaluate() override;

        model::InputSlot* getInput() const;
    };
}