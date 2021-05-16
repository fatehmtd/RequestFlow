#include "Node.h"
#include "Graph.h"
#include "Slot.h"
#include <QChildEvent>
#include <QDebug>

model::Node::Node(Graph* parent, const QString& typeName) : NotifiableEntity(parent)
{
	setType(typeName);
	connect(parent, &Graph::started, this, [this]() 
		{
			onGraphStart();
		}, Qt::ConnectionType::QueuedConnection);

	connect(parent, &Graph::stopped, this, [this]()
		{
			onGraphStop();
		}, Qt::ConnectionType::QueuedConnection);

	//connect(this, &Node::readyForEvaluation, this, &Node::evaluate);
	connect(this, &Node::evaluated, parent, &Graph::onNodeEvaluated, Qt::ConnectionType::QueuedConnection);
    connect(this, &Node::ready, parent, &Graph::onNodeReady, Qt::ConnectionType::QueuedConnection);
	connect(this, &Node::failed, parent, &Graph::onNodeFailed, Qt::ConnectionType::QueuedConnection);
	connect(this, &Node::failed, parent, &Graph::onNodeFailed, Qt::ConnectionType::QueuedConnection);
	//connect(this, &Node::exceptionRaised, parent, &Graph::onNodeException, Qt::ConnectionType::QueuedConnection);
}

model::InputSlot* model::Node::getDestinationSlot(const QString& name) const
{
	for (auto slot : findChildren<InputSlot*>())
	{
		if (slot->getName() == name) return slot;
	}
	return nullptr;
}

model::OutputSlot* model::Node::getOriginSlot(const QString& name) const
{
	for (auto slot : findChildren<OutputSlot*>())
	{
		if (slot->getName() == name) return slot;
	}
	return nullptr;
}

model::InputSlot* model::Node::getDestinationSlotByIdentifier(const QString& id) const
{
	return findChild<InputSlot*>(id, Qt::FindChildOption::FindDirectChildrenOnly);
}

model::OutputSlot* model::Node::getOriginSlotByIdentifier(const QString& id) const
{
	return findChild<OutputSlot*>(id, Qt::FindChildOption::FindDirectChildrenOnly);
}

model::InputSlot* model::Node::addInputSlot(QString name, int dataType)
{
	auto slot = new InputSlot(this, name, dataType);
	connect(slot, &InputSlot::dataReceived, this, &Node::slotDataReceived, Qt::ConnectionType::QueuedConnection);
	connect(slot, &InputSlot::failed, this, &Node::slotFailed, Qt::ConnectionType::QueuedConnection);
	return slot;
}

model::OutputSlot* model::Node::addOutputSlot(QString name, int dataType)
{
	auto slot = new OutputSlot(this, name, dataType);
	connect(this, &Node::evaluated, slot, [slot]() { slot->sendData(); }, Qt::ConnectionType::QueuedConnection);
	connect(this, &Node::failed, slot, &OutputSlot::onNodeFail, Qt::ConnectionType::QueuedConnection);
	connect(slot, &OutputSlot::dataSent, this, &Node::slotDataSent, Qt::ConnectionType::QueuedConnection);
	return slot;
}

QMap<QString, model::InputSlot*> model::Node::getInputSlotsMap() const
{
	QMap<QString, InputSlot*> output;
	auto listOfChildren = findChildren<InputSlot*>();
	for (auto child : listOfChildren)
	{
		auto slot = dynamic_cast<InputSlot*>(child);
		if (slot != nullptr)
		{
			output[slot->getIdentifier()] = slot;
		}
	}
    return output;
}

QMap<QString, model::OutputSlot*> model::Node::getOutputSlotsMap() const
{
	QMap<QString, OutputSlot*> output;
	auto listOfChildren = findChildren<OutputSlot*>();
	for (auto child : listOfChildren)
	{
		auto slot = dynamic_cast<OutputSlot*>(child);
		if (slot != nullptr)
		{
			output[slot->getIdentifier()] = slot;
		}
	}
    return output;
}

QList<model::InputSlot*> model::Node::getInputSlots() const
{
	return findChildren<InputSlot*>();
}

QList<model::OutputSlot*> model::Node::getOutputSlots() const
{
	return findChildren<OutputSlot*>();
}

model::Graph* model::Node::getGraph() const
{
	return dynamic_cast<Graph*>(parent());
}

void model::Node::clear()
{
	setStatus(IDLE);
	_listOfReadySlots.clear();
}

QJSValue model::Node::saveToJSValue(PersistenceHandler* persistenceHandler) const
{
	auto value = PersistableEntity::saveToJSValue(persistenceHandler);

	saveChildren<InputSlot*>(value, persistenceHandler, "inputSlots", getInputSlots());
	saveChildren<OutputSlot*>(value, persistenceHandler, "outputSlots", getOutputSlots());

	return value;
}

bool model::Node::loadFromJSValue(const QJSValue& v)
{
	PersistableEntity::loadFromJSValue(v);

	qDeleteAll(getInputSlots());
	qDeleteAll(getInputSlots());

	loadChildren(v, "inputSlots", [=](const QJSValue& value) 
		{
			auto inputSlot = addInputSlot("", -1);
			inputSlot->loadFromJSValue(value);
		});

	loadChildren(v, "outputSlots", [=](const QJSValue& value) 
		{
			auto outputSlot = addOutputSlot("", -1);
			outputSlot->loadFromJSValue(value);
		});

	return true;
}

void model::Node::evaluate()
{
	if (!getGraph()->isRunning()) return;

    emit evaluated();
    setStatus(EVALUATED);

    const auto& values = getOutputSlotsMap().values();
    for (auto slot : values)
    {
        slot->sendData();
    }
}

int model::Node::getStatus() const
{
    return _executionStatus;
}

void model::Node::prepareNodeInternals()
{

}

void model::Node::setStatus(int status)
{
	_executionStatus = status;
}

void model::Node::fail(const QString& reason)
{
	emit failed(reason);
}

void model::Node::onGraphStart()
{
	clear();

	// If the node has no input slots, proceed to evaluation when the graph execution starts
	if (getInputSlotsMap().size() == 0)
	{
		emit ready();
	}
}

void model::Node::onGraphStop()
{
	clear();
}

void model::Node::dataReceived(InputSlot* slot)
{

}

void model::Node::slotDataReceived() 
{
	if (!getGraph()->isRunning()) return;
	auto inputSlot = dynamic_cast<InputSlot*>(sender());
	if (inputSlot != nullptr)
	{
		if (!_listOfReadySlots.contains(inputSlot))
		{
			dataReceived(inputSlot);

			_listOfReadySlots.push_back(inputSlot);
			
			// When all the slots receive data, the node becomes ready for evaluation
			if (_listOfReadySlots.size() == getInputSlotsMap().size())
			{
				emit ready();
			}
		}
	}
}

void model::Node::slotDataSent() 
{

}

void model::Node::slotFailed()
{
	setStatus(FAILED);
	fail("Failed from above");
}
