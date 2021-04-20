#include "PayloadNode.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

model::PayloadNode::PayloadNode(model::Graph* graph) : model::Node(graph, "Payload")
{

}

void model::PayloadNode::createModel()
{
	addOutputSlot("output", Slot::CUSTOM);
}

void model::PayloadNode::setMessage(const model::Message& message)
{
	_message = message;
}

model::Message model::PayloadNode::getMessage() const
{
	return _message;
}

void model::PayloadNode::evaluate()
{
	getOutputSlot()->setData(getMessage());
	Node::evaluate();
}

model::OutputSlot* model::PayloadNode::getOutputSlot() const
{
	return getOutputSlots().values()[0];
}

QVariant model::PayloadNode::messageToVariant() const
{
	return _message.toVariant();
}

void model::PayloadNode::messageFromVariant(const QVariant& v)
{
	_message.fromVariant(v);
}
