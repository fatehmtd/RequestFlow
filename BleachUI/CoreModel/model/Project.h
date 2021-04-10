#pragma once

#include "../coremodel_global.h" 
#include <QObject>

namespace model
{
	class Environment;
	class Graph;

	class COREMODEL_EXPORT Project : public QObject
	{
	public:
		Project(QObject* parent);
		~Project();

		QList<Graph*> getGraphs() const;
		QList<Environment*> getEnvironments() const;
	};
}