#include "Edge.h"
#include "Graph.h"
#include "Slot.h"
#include <QDebug>
#include "Node.h"
#include "Slot.h"

model::Edge::Edge(Graph* parent) : NotifiableEntity(parent)
{
	setType("Edge");
}

bool model::Edge::setDestination(InputSlot* slot)
{
	if (_originSlot != nullptr)
	{
		// check if the same data type
		if (slot->getDataType() != _originSlot->getDataType()) return false;
	}
	_destinationSlot = slot;

	connect(this, &Edge::dataReceived, _destinationSlot, &InputSlot::onDataReceived, Qt::ConnectionType::DirectConnection);

	return true;
}

bool model::Edge::setOrigin(OutputSlot* slot)
{
	if (_destinationSlot != nullptr)
	{
		// check if the same data type
		if (slot->getDataType() != _destinationSlot->getDataType()) return false;
	}
	_originSlot = slot;
	connect(_originSlot, &OutputSlot::dataSent, this, &Edge::onDataReceived, Qt::ConnectionType::DirectConnection);

	return true;
}

model::InputSlot* model::Edge::getDestinationSlot() const
{
	return _destinationSlot;
}

model::OutputSlot* model::Edge::getOriginSlot() const
{
	return _originSlot;
}

QJSValue model::Edge::saveToJSValue(PersistenceHandler* handler) const
{
	auto value = PersistableEntity::saveToJSValue(handler);

	auto originValue = handler->createJsValue();
	originValue.setProperty("node", handler->createJsValue(getOriginSlot()->getNode()->getIdentifier()));
	originValue.setProperty("slot", handler->createJsValue(getOriginSlot()->getIdentifier()));

	auto destinationValue = handler->createJsValue();
	destinationValue.setProperty("node", handler->createJsValue(getDestinationSlot()->getNode()->getIdentifier()));
	destinationValue.setProperty("slot", handler->createJsValue(getDestinationSlot()->getIdentifier()));

	value.setProperty("origin", originValue);
	value.setProperty("destination", destinationValue);
	return value;
}

model::Slot* getSlot(const QJSValue& slotValue, model::Graph* graph)
{
	auto nodeId = slotValue.property("node").toString();
	auto slotId = slotValue.property("slot").toString();

	auto node = graph->findChild<model::Node*>(nodeId);
	auto slot = node->findChild<model::Slot*>(slotId);
	return slot;
}

bool model::Edge::loadFromJSValue(const QJSValue& v)
{
	PersistableEntity::loadFromJSValue(v);
	auto graph = dynamic_cast<Graph*>(parent());
	setOrigin((OutputSlot*)getSlot(v.property("origin"), graph));
	setDestination((InputSlot*)getSlot(v.property("destination"), graph));
	return true;
}

void model::Edge::onDataReceived()
{
	if (_originSlot != nullptr && _destinationSlot != nullptr)
	{
		_destinationSlot->setData(_originSlot->getData());
		emit dataReceived();
	}
}