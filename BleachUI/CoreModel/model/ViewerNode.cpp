#include "ViewerNode.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

model::ViewerNode::ViewerNode(model::Graph* graph) : model::Node(graph, "Viewer")
{

}

#include <QDebug>

void model::ViewerNode::createModel()
{
	addInputSlot("in", model::Slot::DataType::CUSTOM);
}

void model::ViewerNode::evaluate()
{
	Node::evaluate();
}

model::InputSlot* model::ViewerNode::getInput() const
{
	return getInputSlots().values().first();
}
