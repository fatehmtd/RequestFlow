#include "Project.h"

#include "Environment.h"
#include "Graph.h"

model::Project::Project(QObject* parent) : QObject(parent)
{
}

model::Project::~Project()
{

}

QList<model::Graph*> model::Project::getGraphs() const
{
    return findChildren<Graph*>();
}

QList<model::Environment*> model::Project::getEnvironments() const
{
    return findChildren<Environment*>();
}
