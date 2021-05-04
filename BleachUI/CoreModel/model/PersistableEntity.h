#pragma once
#include <QObject>
#include "PersistenceHandler.h"
#include <functional>

namespace model
{
	class COREMODEL_EXPORT PersistableEntity : public QObject
	{
	public:
		using QObject::QObject;
		virtual QJSValue saveToJSValue(PersistenceHandler* persistenceHandler) const;
		virtual bool loadFromJSValue(const QJSValue& value);
	protected:		

		template <typename T>
		void saveChildren(QJSValue& out, 
			PersistenceHandler* persistenceHandler,
			const QString& name,
			const QList<T>& children, 
			std::function<QJSValue (T)> func) const
		{
			auto jsArray = persistenceHandler->createJsValueArray(children.size());
			for (int i = 0; i < children.size(); i++)
			{
				auto value = func(children[i]);
				jsArray.setProperty(i, value);
			}
			out.setProperty(name, jsArray);
		}

		template <typename T>
		void saveChildren(QJSValue& out, 
			PersistenceHandler* persistenceHandler,
			const QString& name,
			const QList<T>& children) const
		{
			auto jsArray = persistenceHandler->createJsValueArray(children.size());
			for (int i = 0; i < children.size(); i++)
			{
				auto value = children[i]->saveToJSValue(persistenceHandler);
				jsArray.setProperty(i, value);
			}
			out.setProperty(name, jsArray);
		}


		virtual void loadChildren(const QJSValue& value, const QString& name, std::function<void(const QJSValue&)> func);
	};
}
