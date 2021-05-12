#pragma once
#include "../coremodel_global.h"
#include <QJSValue>
#include <QJSEngine>
#include <QJsonDocument>
#include <QPointF>

namespace model
{
	class COREMODEL_EXPORT PersistenceHandler
	{
	public:
		PersistenceHandler();
		QJSValue createJsValue();
		QJSValue createJsValue(const QVariant& v);
		QJSValue createJsValue(const QPointF& v);
		QJSValue createJsValueArray(quint32 size);

		QPointF jsValueToPointF(const QJSValue& v) const;

		template<typename T>
		QJSValue createJsValue(const T& v)
		{
			return _engine.toScriptValue<T>(v);
		}

		QString evaluate(const QJSValue& v);
		QJSValue evaluate(const QString& script);
	private:
		QJSEngine _engine;
	};
}
