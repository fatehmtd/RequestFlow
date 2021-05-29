#include "Slot.h"
#include "Node.h"
#include <QDebug>

model::Slot::Slot(Node* parent, QString name, Direction direction, int dataType) : NotifiableEntity(parent), _dataType(dataType), _direction(direction), _data("")
{
    setName(name);
}

model::Slot::~Slot()
{
}

void model::Slot::clear()
{
    _data = model::Message("");
}

model::Node* model::Slot::getNode() const
{
    return dynamic_cast<Node*>(parent());
}

int model::Slot::getDirection() const
{
    return _direction;
}

int model::Slot::getDataType() const
{
    return _dataType;
}

model::Message model::Slot::getData() const
{
    return _data;
}

void model::Slot::setData(const Message& data)
{
    _data = data;
}

//////////////////////////////////////////////////////////////////////////

model::InputSlot::InputSlot(Node* parent, QString name, int dataType) : Slot(parent, name, Slot::Direction::INPUT, dataType)
{
    setType("InputSlot");
    //connect(this, &InputSlot::dataReceived, this, &InputSlot::onDataReceived);
}

void model::InputSlot::onDataReceived()
{
    emit dataReceived();
}

//////////////////////////////////////////////////////////////////////////

model::OutputSlot::OutputSlot(Node* parent, QString name, int dataType) : Slot(parent, name, Slot::Direction::OUTPUT, dataType)
{
    setType("OutputSlot");
}

void model::OutputSlot::sendData()
{
    emit dataSent();
}

void model::OutputSlot::onNodeFail()
{
    emit failed();
}

void model::InputSlot::onFailed()
{
    emit failed();
}
