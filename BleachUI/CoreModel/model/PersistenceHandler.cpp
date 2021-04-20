#include "PersistenceHandler.h"

model::PersistenceHandler::PersistenceHandler()
{
}

QJSValue model::PersistenceHandler::createJsValue()
{
	return _engine.newObject();
}

QJSValue model::PersistenceHandler::createJsValue(const QPointF& p)
{
	auto value = createJsValue();
	value.setProperty("x", p.x());
	value.setProperty("y", p.y());
	return value;
}

QJSValue model::PersistenceHandler::createJsValue(const QVariant& v)
{
	return _engine.toScriptValue<QVariant>(v);
}

QJSValue model::PersistenceHandler::createJsValueArray(quint32 size)
{
	return _engine.newArray(size);
}

QString model::PersistenceHandler::evaluate(const QJSValue& v)
{
	return QJsonDocument::fromVariant(_engine.fromScriptValue<QVariant>(v)).toJson(QJsonDocument::JsonFormat::Compact);
}

QJSValue model::PersistenceHandler::evaluate(const QString& script)
{
	return _engine.evaluate(script);
}

QPointF model::PersistenceHandler::jsValueToPointF(const QJSValue& v) const
{
	return QPointF(v.property("x").toNumber(), v.property("y").toNumber());
}