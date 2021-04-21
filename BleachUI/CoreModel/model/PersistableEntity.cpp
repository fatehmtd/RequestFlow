#include "PersistableEntity.h"
#include <QMetaProperty>
#include <QMetaObject>

QJSValue model::PersistableEntity::saveToJSValue(PersistenceHandler* persistenceHandler) const
{
	QJSValue value = persistenceHandler->createJsValue();

	for (int i = 0; i < metaObject()->propertyCount(); i++)
	{
		auto prop = metaObject()->property(i);
		value.setProperty(QString(prop.name()), persistenceHandler->createJsValue(property(prop.name())));
	}

	return value;
}

bool model::PersistableEntity::loadFromJSValue(const QJSValue& value)
{
	for (int i = 0; i < metaObject()->propertyCount(); i++)
	{
		auto prop = metaObject()->property(i);
		setProperty(prop.name(), value.property(QString(prop.name())).toVariant());
	}
	return true;
}

void model::PersistableEntity::saveChildren(QJSValue& out, PersistenceHandler* persistenceHandler,
	QString name, const QList<PersistableEntity*>& children) const
{
	saveChildren(out, persistenceHandler, name, children.toVector());
}

void model::PersistableEntity::saveChildren(QJSValue& out, PersistenceHandler* persistenceHandler,
	QString name, const QVector<PersistableEntity*>& children) const
{
	saveChildren(out, persistenceHandler, name, children.data(), children.size());
}

void model::PersistableEntity::saveChildren(QJSValue& out, PersistenceHandler* persistenceHandler, QString name, PersistableEntity** children, int count) const
{
	saveChildren(out, persistenceHandler, name, (PersistableEntity* const*)children, count);
}

void model::PersistableEntity::saveChildren(QJSValue& out, PersistenceHandler* persistenceHandler, QString name, PersistableEntity* const * children, int count) const
{
	auto valuesArray = persistenceHandler->createJsValueArray(count);
	for (int i = 0; i < count; i++)
	{
		auto value = children[i]->saveToJSValue(persistenceHandler);
		valuesArray.setProperty(i, value);
	}
	out.setProperty(name, valuesArray);
}
