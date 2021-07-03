#pragma once
#include <QObject>
#include "PersistenceHandler.h"
#include <functional>

namespace model
{
    class COREMODEL_EXPORT PersistableEntity
	{
    public:
        PersistableEntity(QObject* owner);
		virtual QJSValue saveToJSValue(PersistenceHandler* persistenceHandler) const;
		virtual bool loadFromJSValue(const QJSValue& value);

        QObject *getObject() const;
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

		template <typename K, typename T>
		void saveChildren(QJSValue& out, 
			PersistenceHandler* persistenceHandler,
			const QString& name,
			const QMap<K, T>& children, 
			std::function<QJSValue (const K&, const T&)> func) const
		{
			auto jsArray = persistenceHandler->createJsValueArray(children.size());
			
			int index = 0;
			for(const auto& k : children.keys())
			{
				auto jsPair = persistenceHandler->createJsValue();
				jsPair.setProperty(k, func(k, children[k]));
				jsArray.setProperty(index++, jsPair);
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
    private:
        QObject * _object = nullptr;
	};
}
