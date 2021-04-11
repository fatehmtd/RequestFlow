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
    public:
		AssertionNode(model::Graph* graph);
        
        void createModel() override;

		InputSlot* getInputSlot() const;
		OutputSlot* getOutputSlot() const;
    private:
        InputSlot* _inputSlot = nullptr;
        OutputSlot* _outputSlot = nullptr;
    };
}
