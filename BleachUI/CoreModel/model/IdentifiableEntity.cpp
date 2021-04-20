#include "IdentifiableEntity.h"
#include <QUUID>

model::IdentifiableEntity::IdentifiableEntity(QObject* parent) : PersistableEntity(parent)
{
	setIdentifier(QUuid::createUuid().toString()); // auto generate an id
}

void model::IdentifiableEntity::setIdentifier(QString id)
{
	setObjectName(id);
	emit identifierChanged(id);
}

QString model::IdentifiableEntity::getIdentifier() const
{
	return objectName();
}

void model::IdentifiableEntity::setType(QString type)
{
	_type = type;
}

QString model::IdentifiableEntity::getType() const
{
	return _type;
}

void model::IdentifiableEntity::setName(QString name)
{
	_name = name;
	emit nameChanged(name);
}

QString model::IdentifiableEntity::getName() const
{
	return _name;
}
