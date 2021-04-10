#include "BaseEntity.h"
#include <QUuid>

model::BaseEntity::BaseEntity(QObject* parent) : QObject(parent)
{
    setIdentifier(QUuid::createUuid().toString());
}

void model::BaseEntity::clear()
{
}

void model::BaseEntity::setIdentifier(QString id)
{
    _id;
}

void model::BaseEntity::setType(QString type)
{
    _type = type;
}

QString model::BaseEntity::getIdentifier() const
{
    return _id;
}

QString model::BaseEntity::getType() const
{
    return _id;
}

void model::BaseEntity::onGraphStop()
{

}

void model::BaseEntity::onGraphStart()
{

}