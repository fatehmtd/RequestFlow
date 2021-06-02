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
        auto v = value.property(QString(prop.name())).toVariant();
		setProperty(prop.name(), v);
	}
	return true;
}

void model::PersistableEntity::loadChildren(const QJSValue& value, const QString& name, std::function<void(const QJSValue&)> func)
{
	auto arrayValue = value.property(name);
	for (int i = 0; i < arrayValue.property("length").toInt(); i++)
	{
		func(arrayValue.property(i));
	}
}
