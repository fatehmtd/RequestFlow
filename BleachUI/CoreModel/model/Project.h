#pragma once

#include "IdentifiableEntity.h"

namespace model
{
	class Environment;
	class Graph;

	class COREMODEL_EXPORT Project : public IdentifiableEntity
	{
	public:
		Project(QObject* parent);
		~Project();

		QList<Graph*> getGraphs() const;
		QList<Environment*> getEnvironments() const;
	};
}