#pragma once

#include "IdentifiableEntity.h"

namespace model
{
	class Environment;
	class Graph;
	class Document;

	class COREMODEL_EXPORT Project : public IdentifiableEntity
	{
	public:
		Project(QObject* parent);
		~Project();

		QList<Graph*> getGraphs() const;
		QList<Environment*> getEnvironments() const;
		QList<Document*> getDocuments() const;

		QJSValue saveToJSValue(PersistenceHandler* persistenceHandler) const override;
		bool loadFromJSValue(const QJSValue& v) override;

		void setPath(const QString& path);
		QString getPath() const;
	protected:
		QString _path;
	};
}