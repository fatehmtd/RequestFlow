#pragma once

#include "../coremodel_global.h"
#include "IdentifiableEntity.h"
#include <QObject>
#include <QMap>
#include <QVariant>

namespace model
{
	class Project;

	class COREMODEL_EXPORT Environment : public IdentifiableEntity
	{
		Q_OBJECT
	public:
		Environment(Project* parent);
		Environment(const Environment& original);

		Project* getProject() const;

		void setEntries(const QMap<QString, QVariant>& entries);
		QMap<QString, QVariant>& getEntries();
		QMap<QString, QVariant> getEntries() const;

		QString evaluate(QString input) const; // replaces {key} by the corresponding entry

		QJSValue saveToJSValue(PersistenceHandler* persistenceHandler) const override;
		bool loadFromJSValue(const QJSValue& v) override;

	private:
		QMap<QString, QVariant> _entries;
	};
}