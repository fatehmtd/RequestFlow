#include "ViewerNode.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

model::ViewerNode::ViewerNode(model::Graph* graph) : model::Node(graph, "Viewer")
{
	setFilter("$.*");
}

void model::ViewerNode::createModel()
{
	addInputSlot("in", model::Slot::DataType::CUSTOM);
}

model::InputSlot* model::ViewerNode::getInput() const
{
	return getInputSlotsMap().values().first();
}

void model::ViewerNode::setFilter(const QString& filter)
{
	_filter = filter;
}

QString model::ViewerNode::getFilter() const
{
	return _filter;
}
