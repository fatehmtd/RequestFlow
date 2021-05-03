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

#include "Graph.h"
#include "MessageLogger.h"

void model::IdentifiableEntity::success(const QString& message)
{
	auto graph = dynamic_cast<model::Graph*>(parent());
	if (graph == nullptr) graph = dynamic_cast<model::Graph*>(const_cast<model::IdentifiableEntity*>(this));
	if (graph != nullptr)
	{
		graph->getLogger()->log(MessageLogger::MessageType::SUCCESS, message, this);
	}
}

void model::IdentifiableEntity::debug(const QString& message)
{
	auto graph = dynamic_cast<model::Graph*>(parent());
	if (graph == nullptr) graph = dynamic_cast<model::Graph*>(const_cast<model::IdentifiableEntity*>(this));
	if (graph != nullptr)
	{
		graph->getLogger()->log(MessageLogger::MessageType::DEBUG, message, this);
	}
}

void model::IdentifiableEntity::warn(const QString& message)
{
	auto graph = dynamic_cast<model::Graph*>(parent());
	if (graph == nullptr) graph = dynamic_cast<model::Graph*>(const_cast<model::IdentifiableEntity*>(this));
	if (graph != nullptr)
	{
		graph->getLogger()->log(MessageLogger::MessageType::WARNING, message, this);
	}
}

void model::IdentifiableEntity::info(const QString& message)
{
	auto graph = dynamic_cast<model::Graph*>(parent());
	if (graph == nullptr) graph = dynamic_cast<model::Graph*>(const_cast<model::IdentifiableEntity*>(this));
	if (graph != nullptr)
	{
		graph->getLogger()->log(MessageLogger::MessageType::INFO, message, this);
	}
}

void model::IdentifiableEntity::error(const QString& message)
{
	auto graph = dynamic_cast<model::Graph*>(parent());
	if (graph == nullptr) graph = dynamic_cast<model::Graph*>(const_cast<model::IdentifiableEntity*>(this));
	if (graph != nullptr)
	{
		graph->getLogger()->log(MessageLogger::MessageType::ERROR, message, this);
	}
}
