#pragma once
#include <QObject>
#include "PersistenceHandler.h"

namespace model
{
	class COREMODEL_EXPORT PersistableEntity : public QObject
	{
	public:
		using QObject::QObject;
		virtual QJSValue saveToJSValue(PersistenceHandler* persistenceHandler) const;
		virtual bool loadFromJSValue(const QJSValue& value);
	protected:
		virtual void saveChildren(QJSValue& out, PersistenceHandler* persistenceHandler, QString name, const QList<PersistableEntity*>& children) const;
		virtual void saveChildren(QJSValue& out, PersistenceHandler* persistenceHandler, QString name, const QVector<PersistableEntity*>& children) const;
		virtual void saveChildren(QJSValue& out, PersistenceHandler* persistenceHandler, QString name, PersistableEntity* const * children, int count) const;
		virtual void saveChildren(QJSValue& out, PersistenceHandler* persistenceHandler, QString name, PersistableEntity** children, int count) const;
	};
}
