#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <QDebug>
#include "Slot.h"
#include <QSet>

model::Graph::Graph()
{

}

model::Graph::~Graph()
{

}

QList<model::Node*> model::Graph::getNodes() const
{
	return findChildren<Node*>();
}

QList<model::Edge*> model::Graph::getEdges() const
{
	return findChildren<Edge*>();
}

model::Edge* model::Graph::findEdge(const InputSlot* destination, const OutputSlot* origin) const
{
	for (auto edge : getEdges())
	{
		if (edge->getOriginSlot() == origin &&
			edge->getDestinationSlot() == destination)
			return edge;
	}
	return nullptr;
}

QList<model::Edge*> model::Graph::findEdges(const Slot* slot) const
{
	QList<Edge*> edges;

	for (auto edge : getEdges())
	{
		if (edge->getOriginSlot() == slot ||
			edge->getDestinationSlot() == slot)
			edges << edge;
	}
	return std::move(edges);
}

QList<model::Edge*> model::Graph::findEdges(const Node* node) const
{
	QList<Edge*> edgesList;

	for (auto slot : node->getInputSlots().values())
	{
		auto list = findEdges(slot);
		edgesList.append(list);
	}

	for (auto slot : node->getOutputSlots().values())
	{
		auto list = findEdges(slot);
		edgesList.append(list);
	}

	return edgesList.toSet().toList();
}

int model::Graph::start()
{
	int result = computeExecutionPath();
	if (result == Status::OK)
		emit started();
	return result;
}

void model::Graph::stop()
{
	emit stopped();
}

void model::Graph::onNodeEvaluated()
{
	auto node = dynamic_cast<Node*>(sender());
	if (node != nullptr)
	{
		//qDebug() << __FUNCTION__ << node;
	}
}

void model::Graph::onNodeException(QString reason)
{
	auto node = dynamic_cast<Node*>(sender());
	if (node != nullptr)
	{
		emit exceptionRaised(node, reason);
	}

	qDebug() << __FUNCTION__ << reason;
}

int model::Graph::computeExecutionPath()
{
	clear();

	// find starting and ending nodes
	for (auto node : getNodes())
	{
		if (node->getInputSlots().size() == 0)
		{
			_startingNodes << node;
		}

		if (node->getOutputSlots().size() == 0)
		{
			_endingNodes << node;
		}
	}

	if (_startingNodes.isEmpty()) return Status::MISSING_STARTING_NODE;
	if (_endingNodes.isEmpty()) return Status::MISSING_ENDING_NODE;
	if (getEdges().isEmpty()) return Status::MISSING_EDGES;

	return Status::OK;
}

void model::Graph::clear()
{
	for (auto node : _endingNodes)
	{
	}

	_startingNodes.clear();
	_endingNodes.clear();
}

model::Edge* model::Graph::connectSlots(OutputSlot* origin, InputSlot* destination)
{
	//TODO: implement edge creation logic
	auto edge = new Edge(this);
	edge->setOrigin(origin);
	edge->setDestination(destination);
	return edge;
}

void model::Graph::setEnvContext(const QMap<QString, QVariant>& context)
{
	_envContext = context;
}

QMap<QString, QVariant> model::Graph::getEnvContext() const
{
	return _envContext;
}

QMap<QString, QVariant>& model::Graph::getEnvContext()
{
	return _envContext;
}
