#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include "Environment.h"
#include <QDebug>
#include "Slot.h"
#include <QSet>
#include "Project.h"

model::Graph::Graph(Project* parent) : IdentifiableEntity(parent)
{
	setType("Graph");
}

model::Graph::~Graph()
{

}

model::Project* model::Graph::getProject() const
{
	return dynamic_cast<Project*>(parent());
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
	if (result == ErrorCode::OK)
		emit started();
	return result;
}

void model::Graph::setActiveEnvironment(Environment* env)
{
	_environment = env;
}

model::Environment* model::Graph::getActiveEnvironment() const
{
	return _environment;
}

QJSValue model::Graph::saveToJSValue(PersistenceHandler* persistenceHandler) const
{
	auto value = PersistableEntity::saveToJSValue(persistenceHandler);

	auto nodes = getNodes();
	saveChildren(value, persistenceHandler, "nodes", (PersistableEntity* const*)nodes.toVector().data(), nodes.size());

	auto edges = getEdges();
	saveChildren(value, persistenceHandler, "edges", (PersistableEntity* const*)edges.toVector().data(), edges.size());

	return value;
}

#include "PayloadNode.h"
#include "ViewerNode.h"
#include "EndpointNode.h"
#include "ScriptNode.h"
#include "DelayNode.h"
#include "AssertionNode.h"
#include <QMetaMethod>
#include <QDebug>

bool model::Graph::loadFromJSValue(const QJSValue& v)
{
	PersistableEntity::loadFromJSValue(v);

	QMap<QString, QMetaObject> nodesMap;
	nodesMap["Payload"] = PayloadNode::staticMetaObject;
	nodesMap["Endpoint"] = EndpointNode::staticMetaObject;
	nodesMap["Script"] = ScriptNode::staticMetaObject;
	nodesMap["Viewer"] = ViewerNode::staticMetaObject;
	nodesMap["Delay"] = DelayNode::staticMetaObject;
	nodesMap["Assertion"] = AssertionNode::staticMetaObject;

	auto nodesValue = v.property("nodes");
	for (int i = 0; i < nodesValue.property("length").toInt(); i++)
	{
		auto nodeValue = nodesValue.property(i);
		auto nodeType = nodeValue.property("entityType").toString();
		if (nodesMap.contains(nodeType))
		{
			Node* node = reinterpret_cast<Node*>(nodesMap[nodeType].newInstance(Q_ARG(model::Graph*, this)));
			if (node != nullptr)
			{
				node->loadFromJSValue(nodeValue);
			}
		}
	}

	auto edgesValue = v.property("edges");
	for (int i = 0; i < edgesValue.property("length").toInt(); i++)
	{
		auto edge = new Edge(this);
		edge->loadFromJSValue(edgesValue.property(i));
	}
	return true;
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
		_executionNodes[node] = 1;
		checkExecutionStatus();
	}
}

void model::Graph::onNodeFailed()
{
	auto node = dynamic_cast<Node*>(sender());
	if (node != nullptr)
	{
		_executionNodes[node] = -1;
		checkExecutionStatus();
	}
}

void model::Graph::onNodeException(QString reason)
{
	auto node = dynamic_cast<Node*>(sender());
	if (node != nullptr)
	{
		_executionNodes[node] = -1;
		checkExecutionStatus();
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

	if (_startingNodes.isEmpty()) return ErrorCode::MISSING_STARTING_NODE;
	if (_endingNodes.isEmpty()) return ErrorCode::MISSING_ENDING_NODE;
	if (getEdges().isEmpty()) return ErrorCode::MISSING_EDGES;

	return ErrorCode::OK;
}

void model::Graph::clear()
{
	_executionNodes.clear();
	_startingNodes.clear();
	_endingNodes.clear();
}

void model::Graph::checkExecutionStatus()
{
	if (_executionNodes.size() == getNodes().size())
	{
		emit stopped();
	}
}

model::Edge* model::Graph::connectSlots(OutputSlot* origin, InputSlot* destination)
{
	if (!canConnectSlots(origin, destination)) return nullptr;

	auto edges = findEdges(destination);

	if (!edges.isEmpty()) return nullptr; // fail when the destination slot is already connected
	if (origin->getNode() == destination->getNode()) return nullptr; // fail when the two slots belong to the same node

	auto edge = new Edge(this);
	edge->setOrigin(origin);
	edge->setDestination(destination);
	return edge;
}

bool model::Graph::canConnectSlots(Slot* origin, Slot* destination) const
{
	//TODO: implement edge creation logic
	if (origin->getDirection() == destination->getDirection()) return false; // fail when same type nodes
	if (origin->getNode() == destination->getNode()) return false; // fail when the two slots belong to the same node
	if (origin->getDataType() != destination->getDataType()) return false; // fail if different data types

	auto edgesDestination = findEdges(destination);
	if (!edgesDestination.isEmpty()) return false; // fail when the destination slot is already connected

	return true;
}