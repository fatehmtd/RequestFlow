#include "AssertionNode.h"


void model::AssertionNode::createModel()
{
	_inputSlot = addInputSlot("Input", Slot::CUSTOM);
	_outputSlot = addOutputSlot("Output", Slot::CUSTOM);
}

model::InputSlot* model::AssertionNode::getInputSlot() const
{
	return _inputSlot;
}

model::OutputSlot* model::AssertionNode::getOutputSlot() const
{
	return _outputSlot;
}

model::AssertionNode::AssertionNode(model::Graph* graph) : Node(graph, "Assertion")
{

}