#include "externalnode.h"
#include "Slot.h"

model::ExternalNode::ExternalNode(model::Graph *graph) : model::Node(graph, "External")
{
    setDeltaDelay(60);
    setExecutionCondition(ExecutionCondition::ALWAYS);
    setCustomScript("true");
}

void model::ExternalNode::createModel()
{
    addOutputSlot("Output", model::Slot::CUSTOM);
}

void model::ExternalNode::evaluate()
{
    if(shouldRunExternal() || !_firstTimeDataSet)
    {
        // start the external graph
        if(_linkedNode != nullptr)
        {
            auto externalGraph = _linkedNode->getGraph();
            if(_linkedNode->getGraph()->getActiveEnvironment() == nullptr)
            {
                _linkedNode->getGraph()->setActiveEnvironment(getGraph()->getActiveEnvironment());
            }
            externalGraph->start();
        }
        else
        {
            fail("Not connected to an external node");
        }
    }
    else
    {
        emit evaluated();
    }
}

model::OutputSlot *model::ExternalNode::getOutputSlot() const
{
    return getOutputSlots()[0];
}

void model::ExternalNode::setExecutionCondition(int condition)
{
    _executionCondition = condition;
}

int model::ExternalNode::getExecutionCondition() const
{
    return _executionCondition;
}

void model::ExternalNode::setLinkedNode(model::Node *node)
{
    // if already set, unset and remove connections
    if(_linkedNode != nullptr)
    {
        auto output = _linkedNode->getOutputSlots()[0];
        disconnect(output, nullptr, this, nullptr);
        _linkedNode = nullptr;
    }

    if(node != nullptr)
    {
        // set and connect the node
        _linkedNode = node;
        auto output = _linkedNode->getOutputSlots()[0];
        connect(output, &model::OutputSlot::dataSent, this, &ExternalNode::onExternalNodeDataReceived);

        setExternalGraphId(node->getGraph()->getIdentifier());
        setExternalNodeId(node->getIdentifier());
    }
}

model::Node *model::ExternalNode::getLinkedNode() const
{
    return _linkedNode;
}

void model::ExternalNode::setDeltaDelay(quint64 delay)
{
    _deltaDelay = delay;
}

quint64 model::ExternalNode::getDeltaDelay() const
{
    return _deltaDelay;
}

void model::ExternalNode::setCustomScript(const QString &script)
{
    _customScript = script;
}

QString model::ExternalNode::getCustomScript() const
{
    return _customScript;
}

QString model::ExternalNode::getExternalGraphId() const
{
    return _externalGraphId;
}

QString model::ExternalNode::getExternalNodeId() const
{
    return _externalNodeId;
}

#include "Project.h"

void model::ExternalNode::prepareNodeInternals()
{
    auto graph = getGraph()->getProject()->findChild<model::Graph*>(getExternalGraphId(), Qt::FindChildOption::FindDirectChildrenOnly);
    if(graph != nullptr)
    {
        auto node = graph->findChild<model::Node*>(getExternalNodeId());
        if(node != nullptr)
        {
            setLinkedNode(node);
        }
    }
}

void model::ExternalNode::onExternalNodeDataReceived()
{
    const auto outputSlot = _linkedNode->getOutputSlots()[0];
    getOutputSlot()->setData(outputSlot->getData());
    _lastExecutionTimePoint = std::chrono::steady_clock::now();
    _firstTimeDataSet = true;
    emit evaluated();
}

bool model::ExternalNode::shouldRunExternal() const
{
    switch(_executionCondition)
    {
    case RESULT_IS_OLD:
        return checkDelay();
    case CUSTOM:
        return evalScript();
    default: // Always
        return true;
    }
}

bool model::ExternalNode::checkDelay() const
{
    auto duration = std::chrono::steady_clock::now() - _lastExecutionTimePoint;
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count() >= _deltaDelay;
}

bool model::ExternalNode::evalScript() const
{
    //TODO: implement the custom script evaluation
    return true;
}

void model::ExternalNode::setExternalGraphId(const QString &id)
{
    _externalGraphId = id;
}

void model::ExternalNode::setExternalNodeId(const QString &id)
{
    _externalNodeId = id;
}
