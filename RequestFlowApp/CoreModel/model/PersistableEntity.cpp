#include "PersistableEntity.h"
#include <QMetaProperty>
#include <QMetaObject>

model::PersistableEntity::PersistableEntity(QObject *object) : _object(object)
{
    Q_ASSERT_X(object == nullptr, "PersistableEntity", "QObject cannot be NULL");
}

QJSValue model::PersistableEntity::saveToJSValue(PersistenceHandler* persistenceHandler) const
{
	QJSValue value = persistenceHandler->createJsValue();

    for (int i = 0; i < _object->metaObject()->propertyCount(); i++)
	{
        auto prop = _object->metaObject()->property(i);
        value.setProperty(QString(prop.name()), persistenceHandler->createJsValue(_object->property(prop.name())));
	}

	return value;
}

bool model::PersistableEntity::loadFromJSValue(const QJSValue& value)
{
    for (int i = 0; i < _object->metaObject()->propertyCount(); i++)
	{
        auto prop = _object->metaObject()->property(i);
        auto v = value.property(QString(prop.name())).toVariant();
        _object->setProperty(prop.name(), v);
    }
    return true;
}

QObject *model::PersistableEntity::getObject() const
{
    return _object;
}

void model::PersistableEntity::loadChildren(const QJSValue& value, const QString& name, std::function<void(const QJSValue&)> func)
{
	auto arrayValue = value.property(name);
	for (int i = 0; i < arrayValue.property("length").toInt(); i++)
	{
		func(arrayValue.property(i));
	}
}
