#include "Edge.h"
#include "Graph.h"
#include "Slot.h"
#include <QDebug>

model::Edge::Edge(Graph* parent) : BaseEntity(parent)
{

}

bool model::Edge::setDestination(InputSlot* slot)
{
	if (_originSlot != nullptr)
	{
		// check if the same data type
		if (slot->getDataType() != _originSlot->getDataType()) return false;
	}
	_destinationSlot = slot;

	connect(this, &Edge::dataReceived, _destinationSlot, &InputSlot::onDataReceived);
	
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
	connect(_originSlot, &OutputSlot::dataSent, this, &Edge::onDataReceived);

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

void model::Edge::onDataReceived()
{
	if (_originSlot != nullptr && _destinationSlot != nullptr)
	{
		_destinationSlot->setData(_originSlot->getData());
		emit dataReceived();
	}
}