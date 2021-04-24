#include "Project.h"

#include "Environment.h"
#include "Graph.h"
#include "Document.h"
#include <Qdebug>

#include <QMetaProperty>
#include <QMetaObject>

model::Project::Project(QObject* parent) : IdentifiableEntity(parent)
{
	setType("Project");
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

QList<model::Document*> model::Project::getDocuments() const
{
	return findChildren<Document*>();
}

QJSValue model::Project::saveToJSValue(PersistenceHandler* persistenceHandler) const
{
	auto value = PersistableEntity::saveToJSValue(persistenceHandler);

	auto environments = getEnvironments();
	saveChildren(value, persistenceHandler, "environments", (PersistableEntity**)environments.toVector().data(), environments.size());
	
	auto graphs = getGraphs();
	saveChildren(value, persistenceHandler, "graphs", (PersistableEntity**)graphs.toVector().data(), graphs.size());
	
	auto documents = getDocuments();
	saveChildren(value, persistenceHandler, "documents", (PersistableEntity**)documents.toVector().data(), documents.size());

	return value;
}

bool model::Project::loadFromJSValue(const QJSValue& v)
{
	PersistableEntity::loadFromJSValue(v);

	auto envValue = v.property("environments");
	for (int i = 0; i < envValue.property("length").toInt(); i++)
	{
		auto env = new model::Environment(this);
		env->loadFromJSValue(envValue.property(i));
	}

	auto graphsValue = v.property("graphs");
	for (int i = 0; i < graphsValue.property("length").toInt(); i++)
	{
		auto graph = new model::Graph(this);
		graph->loadFromJSValue(graphsValue.property(i));
	}

	auto documentsValue = v.property("documents");
	for (int i = 0; i < documentsValue.property("length").toInt(); i++)
	{
		auto document = new model::Document(this);
		document->loadFromJSValue(documentsValue.property(i));
	}

	return true;
}

void model::Project::setPath(const QString& path)
{
	_path = path;
}

QString model::Project::getPath() const
{
	return _path;
}
