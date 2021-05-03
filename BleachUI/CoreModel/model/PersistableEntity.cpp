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
	//qDebug() << __FUNCTION__;
	for (int i = 0; i < metaObject()->propertyCount(); i++)
	{
		auto prop = metaObject()->property(i);
		//qDebug() << prop.name() << prop.typeName() << prop.type() << prop.userType();
		setProperty(prop.name(), value.property(QString(prop.name())).toVariant());
	}
	return true;
}
/*
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
*/
void model::PersistableEntity::loadChildren(const QJSValue& value, const QString& name, std::function<void(const QJSValue&)> func)
{
	auto arrayValue = value.property(name);
	for (int i = 0; i < arrayValue.property("length").toInt(); i++)
	{
		func(arrayValue.property(i));
	}
}
