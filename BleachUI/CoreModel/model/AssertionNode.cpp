#include "AssertionNode.h"


void model::AssertionNode::createModel()
{
	addInputSlot("Input", Slot::CUSTOM);
	addOutputSlot("Output", Slot::CUSTOM);
}

model::InputSlot* model::AssertionNode::getInputSlot() const
{
	return getInputSlots().values()[0];
}

model::OutputSlot* model::AssertionNode::getOutputSlot() const
{
	return getOutputSlots().values()[0];
}

model::AssertionNode::AssertionNode(model::Graph* graph) : Node(graph, "Assertion")
{

}