#include "Document.h"
#include "EndpointEntry.h"
#include "Project.h"
#include <QDebug>

model::Document::Document(Project* project) : IdentifiableEntity(project)
{
	setType("Document");
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

QJSValue model::Document::saveToJSValue(PersistenceHandler* handler) const
{
	auto value = PersistableEntity::saveToJSValue(handler);
	saveChildren(value, handler, "endpoints", (PersistableEntity* const*)getEndpoints().toVector().data(), getEndpoints().size());
	return value;
}

bool model::Document::loadFromJSValue(const QJSValue& v)
{
	PersistableEntity::loadFromJSValue(v);
	{
		QJSValue jsEndpoints = v.property("endpoints");
		for (int i = 0; i < jsEndpoints.property("length").toInt(); i++)
		{
			auto jsEndpoint = jsEndpoints.property(i);
			auto endpoint = new EndpointEntry(this);
			endpoint->loadFromJSValue(jsEndpoint);
		}
	}
	return true;
}

bool model::Document::importFromSwagger(const QString& path)
{
	QFile fp(path);

	if (fp.open(QIODevice::ReadOnly))
	{
		PersistenceHandler handler;
		QString content = QString(fp.readAll());
		QJSValue rootValue = handler.evaluate(QString("(%1)").arg(content));
		if (rootValue.isError()) return false;

		setName(rootValue.property("info").property("title").toString());

		QList<QString> httpMethodsMap;
		httpMethodsMap << "get";
		httpMethodsMap << "post";
		httpMethodsMap << "put";
		httpMethodsMap << "delete";

		QList<QString> parameterLocationList;
		parameterLocationList << "path";
		parameterLocationList << "query";
		parameterLocationList << "body";
		parameterLocationList << "formData";

		// paths
		{
			auto jsPaths = rootValue.property("paths");
			QJSValueIterator urlsIterator(jsPaths);
			while (urlsIterator.hasNext())
			{
				urlsIterator.next();
				auto jsPath = urlsIterator.value();
				QString url(urlsIterator.name());
				QJSValueIterator methodsIterator(jsPath);
				// methods
				while (methodsIterator.hasNext())
				{
					if (!methodsIterator.next()) break;
					auto jsMethod = methodsIterator.value();

					auto entry = new EndpointEntry(this);
					entry->setUrl(url);
					entry->setHttpMethod(httpMethodsMap.indexOf(methodsIterator.name()));
					auto operationId = jsMethod.property("operationId").toString();
					entry->setIdentifier(operationId);

					// accepts
					{
						QList<QString> acceptedTypesList;
						auto jsAcceptedTypes = jsMethod.property("consumes");
						QJSValueIterator tempIt(jsAcceptedTypes);
						while (tempIt.hasNext())
						{
							if (!tempIt.next()) break;
							if (!tempIt.value().isString()) continue;
							acceptedTypesList << tempIt.value().toString();
						}

						entry->setConsumptionTypes(acceptedTypesList);
					}

					// produces
					{
						QList<QString> producedTypesList;
						auto jsProducedTypes = jsMethod.property("produces");
						QJSValueIterator tempIt(jsProducedTypes);
						while (tempIt.hasNext())
						{
							if (!tempIt.next()) break;
							if (!tempIt.value().isString()) continue;
							producedTypesList << tempIt.value().toString();
						}

						entry->setProductTypes(producedTypesList);
					}

					// parameters
					{
						QList<QString> queryList, pathList;
						auto jsParameters = jsMethod.property("parameters");
						QJSValueIterator tempIt(jsParameters);
						while (tempIt.hasNext())
						{
							if (!tempIt.next()) break;
							auto jsParameter = tempIt.value();
							auto name = jsParameter.property("name").toString();
							auto location = jsParameter.property("in").toString();
							int locationIndex = parameterLocationList.indexOf(location);

							switch (locationIndex)
							{
							case 0: // path
								pathList << name;
								break;
							case 1: // query
								queryList << name;
								break;
							default: // body, formData
								break;
							}
						}
						entry->setQueryParams(queryList);
						entry->setPathParams(pathList);
					}
				}
			}
		}

		return true;
	}

	return false;
}
