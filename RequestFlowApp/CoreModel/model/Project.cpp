#include "Project.h"

#include "Environment.h"
#include "Graph.h"
#include "Document.h"
#include <QDebug>

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
    auto graphs = findChildren<model::Graph*>();
    //qDebug() << __FUNCTION__ << graphs;
    return graphs;
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

	saveChildren(value, persistenceHandler, "environments", getEnvironments());
	saveChildren(value, persistenceHandler, "graphs", getGraphs());
	saveChildren(value, persistenceHandler, "documents", getDocuments());

	return value;
}

bool model::Project::loadFromJSValue(const QJSValue& v)
{
	PersistableEntity::loadFromJSValue(v);

	loadChildren(v, "environments", [=](const QJSValue& value)
		{
			auto env = new model::Environment(this);
			env->loadFromJSValue(value);
		});

	loadChildren(v, "documents", [=](const QJSValue& value)
		{
            auto document = new model::Document(this);
            document->loadFromJSValue(value);
		});

	loadChildren(v, "graphs", [=](const QJSValue& value)
		{
            auto graph = new model::Graph(this);
            graph->loadFromJSValue(value);
		});


    for(auto graph : getGraphs())
    {
        graph->prepareNodesInternals();
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

bool model::Project::eventFilter(QObject* watched, QEvent* event)
{
	if (dynamic_cast<Graph*>(watched))
	{
		qDebug() << watched << event;
	}
	return QObject::eventFilter(watched, event);
}
