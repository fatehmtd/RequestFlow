#include "Graph.h"
#include "Edge.h"
#include "Environment.h"
#include "MessageLogger.h"
#include "Node.h"
#include "Project.h"
#include "Slot.h"
#include <QDebug>
#include <QSet>

model::Graph::Graph(Project* parent)
    : IdentifiableEntity(parent)
{
    setType("Graph");
    _messageLogger = new MessageLogger(this);
}

model::Graph::~Graph()
{
}

model::Project* model::Graph::getProject() const
{
    return dynamic_cast<Project*>(parent());
}

model::Graph* model::Graph::clone() const
{
    PersistenceHandler handler;
    auto jsValue = saveToJSValue(&handler);
    auto newGraph = new Graph(getProject());
    auto id = newGraph->getIdentifier();
    newGraph->loadFromJSValue(jsValue);
    newGraph->setIdentifier(id);

    newGraph->prepareNodesInternals();
    return newGraph;
}

QList<model::Node*> model::Graph::getNodes() const
{
    return findChildren<Node*>();
}

QList<model::Edge*> model::Graph::getEdges() const
{
    return findChildren<Edge*>();
}

QList<model::Node*> model::Graph::getNodeChildren(Node* node) const
{
    QList<Node*> childrenNodes;
    for (auto slot : node->getOutputSlots()) {
        for (auto edge : findEdges(slot)) {
            auto childNode = edge->getDestinationSlot()->getNode();
            if (childNode != nullptr) {
                childrenNodes << childNode;
            }
        }
    }
    return childrenNodes;
}

QList<model::Node*> model::Graph::getNodeParents(Node* node) const
{
    QList<Node*> parentsNodes;
    for (auto slot : node->getInputSlots()) {
        for (auto edge : findEdges(slot)) {
            auto parentNode = edge->getOriginSlot()->getNode();
            if (parentNode != nullptr) {
                parentsNodes << parentNode;
            }
        }
    }
    return parentsNodes;
}

QList<model::Node*> model::Graph::getStartingNodes() const
{
    QList<Node*> output;
    for (auto node : getNodes()) {
        if (node->getInputSlots().isEmpty()) {
            output << node;
        }
    }
    return output;
}

QList<model::Node*> model::Graph::getEndingNodes() const
{
    QList<Node*> output;
    for (auto node : getNodes()) {
        if (node->getOutputSlots().isEmpty()) {
            output << node;
        }
    }
    return output;
}

bool model::Graph::checkIsParent(Node* child, Node* parent) const
{
    auto parentsList = getNodeParents(child);
    if (parentsList.contains(parent))
        return true;
    for (auto p : qAsConst(parentsList)) {
        if (checkIsParent(p, parent))
            return true;
    }
    return false;
}

model::Edge* model::Graph::findEdge(const InputSlot* destination, const OutputSlot* origin) const
{
    for (auto edge : getEdges()) {
        if (edge->getOriginSlot() == origin && edge->getDestinationSlot() == destination)
            return edge;
    }
    return nullptr;
}

QList<model::Edge*> model::Graph::findEdges(const Slot* slot) const
{
    QList<Edge*> edges;

    for (auto edge : getEdges()) {
        if (edge->getOriginSlot() == slot || edge->getDestinationSlot() == slot)
            edges << edge;
    }
    return edges;
}

#include <QSet>

QList<model::Edge*> model::Graph::findEdges(const Node* node) const
{
    QList<Edge*> edgesList;

    QSet<Edge*> edges;

    for (auto slot : node->getInputSlots()) {
        auto list = findEdges(slot);
        //edgesList.append(list);

        std::for_each(list.begin(), list.end(), [&edges](Edge* edge) {
            edges.insert(edge);
        });
    }

    for (auto slot : node->getOutputSlots()) {
        auto list = findEdges(slot);
        //edgesList.append(list);
        std::for_each(list.begin(), list.end(), [&edges](Edge* edge) {
            edges.insert(edge);
        });
    }

    std::for_each(edges.begin(), edges.end(), [&edgesList](Edge* edge) {
        edgesList << edge;
    });

    return edgesList;
}

int model::Graph::start()
{
    if (isRunning()) {
        error("Already running");
        return ALREADY_RUNNING;
    }

    _currentState = State::IDLE;
    emit preparingStartup();

    int result = computeExecutionPath();
    _isRunning = false;
    switch (result) {
    case ErrorCode::OK:
        emit started();
        _currentState = State::STARTED;
        _elapsedTimer.start();
        info("Starting execution...");
        _isRunning = true;
        break;
    case ErrorCode::MISSING_EDGES:
        error("All nodes need to be connected");
        break;
    case ErrorCode::MISSING_STARTING_NODE:
        error("Missing source nodes");
        break;
    case ErrorCode::MISSING_ENDING_NODE:
        error("Missing sink nodes");
        break;
    }
    return result;
}

bool model::Graph::isRunning() const
{
    return _isRunning;
}

bool model::Graph::wasCanceled() const
{
    return _finalExecutionState == State::CANCELED;
}

bool model::Graph::hasFailed() const
{
    return _finalExecutionState == State::FAILED;
}

bool model::Graph::hasSucceeded()
{
    return _finalExecutionState == State::SUCCEEDED;
}

void model::Graph::setExecutionEngine(ExecutionEngine* engine)
{
    _executionEngine = engine;
}

model::ExecutionEngine* model::Graph::getExecutionEngine() const
{
    return _executionEngine;
}

void model::Graph::setActiveEnvironment(Environment* env)
{
    _environment = env;
    emit activeEnvironmentChanged();
}

model::Environment* model::Graph::getActiveEnvironment() const
{
    return _environment;
}

QJSValue model::Graph::saveToJSValue(PersistenceHandler* persistenceHandler) const
{
    auto value = PersistableEntity::saveToJSValue(persistenceHandler);
    saveChildren<Node*>(value, persistenceHandler, "nodes", getNodes());
    saveChildren<Edge*>(value, persistenceHandler, "edges", getEdges());
    QString activeEnv = getActiveEnvironment() == nullptr ? "" : getActiveEnvironment()->getIdentifier();
    value.setProperty("activeEnvironment", persistenceHandler->createJsValue(activeEnv));
    return value;
}

#include "../ExecutionEngine.h"
#include "AssertionNode.h"
#include "DelayNode.h"
#include "EndpointNode.h"
#include "ExternalNode.h"
#include "PayloadNode.h"
#include "ScriptNode.h"
#include "ViewerNode.h"
#include <QDebug>
#include <QMetaMethod>

bool model::Graph::loadFromJSValue(const QJSValue& v)
{
    PersistableEntity::loadFromJSValue(v);

    loadChildren(v, "nodes", [=](const QJSValue& value) {
        createNodeFromJSValue(value);
    });

    loadChildren(v, "edges", [=](const QJSValue& value) {
        auto edge = new Edge(this);
        edge->loadFromJSValue(value);
    });

    auto activeEnvId = v.property("activeEnvironment").toString();
    setActiveEnvironment(getProject()->findChild<Environment*>(activeEnvId));

    return true;
}

model::Node* model::Graph::createNodeFromJSValue(const QJSValue& value)
{
    // TODO: separate the creation logic from the graph, use a node manager
    QMap<QString, QMetaObject> nodesMap;
    nodesMap["Payload"] = PayloadNode::staticMetaObject;
    nodesMap["Endpoint"] = EndpointNode::staticMetaObject;
    nodesMap["Script"] = ScriptNode::staticMetaObject;
    nodesMap["Viewer"] = ViewerNode::staticMetaObject;
    nodesMap["Delay"] = DelayNode::staticMetaObject;
    nodesMap["Assertion"] = AssertionNode::staticMetaObject;
    nodesMap["External"] = ExternalNode::staticMetaObject;

    auto nodeType = value.property("entityType").toString();

    if (nodesMap.contains(nodeType)) {
        Node* node = reinterpret_cast<Node*>(nodesMap[nodeType].newInstance(Q_ARG(model::Graph*, this)));
        if (node != nullptr) {
            node->loadFromJSValue(value);
            return node;
        }
    }

    return nullptr;
}

model::MessageLogger* model::Graph::getLogger() const
{
    return _messageLogger;
}

void model::Graph::prepareNodesInternals()
{
    for (auto node : getNodes()) {
        node->prepareNodeInternals();
    }
}

void model::Graph::cancel()
{
    if (_isRunning) {
        emit stopped();
        warn("Execution canceled by the user");
    }
    _isRunning = false;
    _currentState = State::CANCELED;
}

void model::Graph::onNodeReady()
{
    auto node = dynamic_cast<Node*>(sender());
    if (node != nullptr) {
        _executionTimes[node] = std::chrono::system_clock::now();
    }
}

void model::Graph::onNodeEvaluated()
{
    auto node = dynamic_cast<Node*>(sender());
    if (node != nullptr) {
        _executionNodes[node] = 1;
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _executionTimes[node]);

        node->success(QString("Executed [%1 : %2 ( %3 ms )]").arg(node->getType()).arg(node->getName()).arg(duration.count()));
    }
    checkExecutionStatus();
    emit advanced();
}

void model::Graph::onNodeFailed(const QString& reason)
{
    auto node = dynamic_cast<Node*>(sender());
    if (node != nullptr) {
        _executionNodes[node] = -1;
        node->error(QString("Failed [%1 : %2] : %3").arg(node->getType()).arg(node->getName()).arg(reason));
    }
    emit advanced();
    checkExecutionStatus();
}

void model::Graph::onNodeException(QString reason)
{
    auto node = dynamic_cast<Node*>(sender());
    if (node != nullptr) {
        _executionNodes[node] = -1;
        emit failed(node, reason);
    }

    //error(reason);
    checkExecutionStatus();
}

int model::Graph::computeExecutionPath()
{
    clear();
    QList<Slot*> unconnectedSlots;
    // find starting and ending nodes
    for (auto node : getNodes()) {
        if (node->getInputSlotsMap().isEmpty()) {
            _startingNodes << node;
        } else {
            for (auto slot : node->getInputSlots()) {
                if (findEdges(slot).isEmpty()) {
                    unconnectedSlots << slot;
                }
            }
        }

        if (node->getOutputSlotsMap().isEmpty()) {
            _endingNodes << node;
        }
    }

    if (_startingNodes.isEmpty())
        return ErrorCode::MISSING_STARTING_NODE;
    if (_endingNodes.isEmpty())
        return ErrorCode::MISSING_ENDING_NODE;
    if (getEdges().isEmpty())
        return ErrorCode::MISSING_EDGES;
    if (!unconnectedSlots.isEmpty())
        return ErrorCode::MISSING_EDGES;

    return ErrorCode::OK;
}

void model::Graph::clear()
{
    _executionNodes.clear();
    _startingNodes.clear();
    _endingNodes.clear();
    _executionTimes.clear();
}

void model::Graph::checkExecutionStatus()
{
    if (_executionNodes.size() == getNodes().size()) {
        int numFails = _executionNodes.values().count(-1);
        if (numFails > 0) {
            error(QString("%1 Nodes failed").arg(numFails));
            _finalExecutionState = State::FAILED;
            emit failed();
        } else {
            info(QString("Execution successful, execution time :%1 ms").arg(_elapsedTimer.elapsed()));
            _finalExecutionState = State::SUCCEEDED;
            emit succeeded();
        }
        _isRunning = false;
        emit stopped();
    }
}

model::Edge* model::Graph::connectSlots(OutputSlot* origin, InputSlot* destination)
{
    if (!canConnectSlots(origin, destination))
        return nullptr;

    auto edges = findEdges(destination);

    if (!edges.isEmpty())
        return nullptr; // fail when the destination slot is already connected
    if (origin->getNode() == destination->getNode())
        return nullptr; // fail when the two slots belong to the same node

    auto edge = new Edge(this);
    edge->setOrigin(origin);
    edge->setDestination(destination);
    return edge;
}

model::Edge* model::Graph::connectSlots(InputSlot* destination, OutputSlot* origin)
{
    return connectSlots(origin, destination);
}

bool model::Graph::canConnectSlots(Slot* origin, Slot* destination) const
{
    if (origin->getDirection() == destination->getDirection())
        return false; // fail when same type nodes
    if (origin->getNode() == destination->getNode())
        return false; // fail when the two slots belong to the same node
    if (origin->getDataType() != destination->getDataType())
        return false; // fail if different data types

    // force DAG (Directed Acyclic Graph)
    if (checkIsParent(origin->getNode(), destination->getNode()) || checkIsParent(destination->getNode(), origin->getNode()))
        return false;

    auto edgesDestination = findEdges(destination);
    if (!edgesDestination.isEmpty())
        return false; // fail when the destination slot is already connected

    return true;
}
