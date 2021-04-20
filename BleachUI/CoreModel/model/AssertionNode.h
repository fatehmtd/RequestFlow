#pragma once
#include "../coremodel_global.h"
#include "Node.h"
#include "Message.h"
#include "Graph.h"
#include "Slot.h"

namespace model
{
    class COREMODEL_EXPORT AssertionNode : public model::Node
    {
        Q_OBJECT
    public:
        Q_INVOKABLE AssertionNode(model::Graph* graph);
        
        void createModel() override;

		InputSlot* getInputSlot() const;
		OutputSlot* getOutputSlot() const;
    };
}
