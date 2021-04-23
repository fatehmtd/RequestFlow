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
	connect(this, &Node::exceptionRaised, parent, &Graph::onNodeEvaluated, Qt::ConnectionType::QueuedConnection);
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
	return slot;
}

model::OutputSlot* model::Node::addOutputSlot(QString name, int dataType)
{
	auto slot = new OutputSlot(this, name, dataType);
	connect(this, &Node::evaluated, slot, [slot]() { slot->sendData(); }, Qt::ConnectionType::QueuedConnection);
	connect(slot, &OutputSlot::dataSent, this, &Node::slotDataSent, Qt::ConnectionType::QueuedConnection);
	return slot;
}

QMap<QString, model::InputSlot*> model::Node::getInputSlots() const
{
	QMap<QString, InputSlot*> output;
	auto listOfChildren = children();
	for (auto child : listOfChildren)
	{
		auto slot = dynamic_cast<InputSlot*>(child);
		if (slot != nullptr)
		{
			output[slot->getIdentifier()] = slot;
		}
	}
	return std::move(output);
}

QMap<QString, model::OutputSlot*> model::Node::getOutputSlots() const
{
	QMap<QString, OutputSlot*> output;
	auto listOfChildren = children();
	for (auto child : listOfChildren)
	{
		auto slot = dynamic_cast<OutputSlot*>(child);
		if (slot != nullptr)
		{
			output[slot->getIdentifier()] = slot;
		}
	}
	return std::move(output);
}

model::Graph* model::Node::getGraph() const
{
	return dynamic_cast<Graph*>(parent());
}

void model::Node::clear()
{
	_listOfReadySlots.clear();
}

QJSValue model::Node::saveToJSValue(PersistenceHandler* persistenceHandler) const
{
	auto value = PersistableEntity::saveToJSValue(persistenceHandler);
	auto inputSlots = getInputSlots();
	auto oututSlots = getOutputSlots();
	saveChildren(value, persistenceHandler, "inputSlots", (PersistableEntity*const*)inputSlots.values().toVector().data(), inputSlots.size());
	saveChildren(value, persistenceHandler, "outputSlots", (PersistableEntity*const*)oututSlots.values().toVector().data(), oututSlots.size());
	return value;
}

bool model::Node::loadFromJSValue(const QJSValue& v)
{
	PersistableEntity::loadFromJSValue(v);

	qDeleteAll(getInputSlots());
	qDeleteAll(getOutputSlots());

	auto inputSlotsValue = v.property("inputSlots");
	for (int i = 0; i < inputSlotsValue.property("length").toInt(); i++)
	{
		auto inputSlot = addInputSlot("", -1);
		inputSlot->loadFromJSValue(inputSlotsValue.property(i));
	}

	auto outputSlotsValue = v.property("outputSlots");
	for (int i = 0; i < outputSlotsValue.property("length").toInt(); i++)
	{
		auto outputSlot = addOutputSlot("", -1);
		outputSlot->loadFromJSValue(outputSlotsValue.property(i));
	}

	return true;
}

void model::Node::evaluate()
{
	emit evaluated();

	for (auto slot : getOutputSlots().values())
	{
		slot->sendData();
	}	
}

void model::Node::raiseException(QString reason)
{
	emit exceptionRaised(reason);
}

void model::Node::onGraphStart()
{
	clear();

	// If the node has no input slots, proceed to evaluation when the graph execution starts
	if (getInputSlots().size() == 0)
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
	auto inputSlot = dynamic_cast<InputSlot*>(sender());
	if (inputSlot != nullptr)
	{
		if (!_listOfReadySlots.contains(inputSlot))
		{
			dataReceived(inputSlot);

			_listOfReadySlots.push_back(inputSlot);
			
			// When all the slots receive data, the node becomes ready for evaluation
			if (_listOfReadySlots.size() == getInputSlots().size())
			{
				emit ready();
			}
		}
	}
}

void model::Node::slotDataSent() 
{

}