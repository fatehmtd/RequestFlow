#include "Document.h"
#include "EndpointEntry.h"
#include "Project.h"

model::Document::Document(Project* project) : IdentifiableEntity(project)
{

}

QMap<QString, model::EndpointEntry*> model::Document::getEndpointsMap() const
{
	QMap<QString, EndpointEntry*> map;
	for (auto e : getEndpoints())
	{
		map[e->getIdentifier()] = e;
	}
	return map;
}

QList<model::EndpointEntry*> model::Document::getEndpoints() const
{
	return findChildren<EndpointEntry*>();
}
