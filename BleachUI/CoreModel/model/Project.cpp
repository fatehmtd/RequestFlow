#include "Project.h"

#include "Environment.h"
#include "Graph.h"
#include <Qdebug>

model::Project::Project(QObject* parent) : IdentifiableEntity(parent)
{
}

model::Project::~Project()
{

}

QList<model::Graph*> model::Project::getGraphs() const
{
	return findChildren<model::Graph*>();
}

QList<model::Environment*> model::Project::getEnvironments() const
{
	return findChildren<model::Environment*>();
}
