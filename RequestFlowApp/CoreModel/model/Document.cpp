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
	saveChildren<EndpointEntry*>(value, handler, "endpoints", getEndpoints());
	return value;
}

bool model::Document::loadFromJSValue(const QJSValue& v)
{
	PersistableEntity::loadFromJSValue(v);

	loadChildren(v, "endpoints", [=](const QJSValue& value)
		{
			auto endpoint = new EndpointEntry(this);
			endpoint->loadFromJSValue(value);
		});

	return true;
}

#include "EndpointNode.h"

bool model::Document::importFromSwagger(const QString& path)
{
	//TODO: refactor and clean
	QFile fp(path);

	if (fp.open(QIODevice::ReadOnly))
	{
		PersistenceHandler handler;
		QString content = QString(fp.readAll());
		QJSValue rootValue = handler.evaluate(QString("(%1)").arg(content));
		if (rootValue.isError()) return false;

		setName(rootValue.property("info").property("title").toString());

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

                    QString httpMethodStr = methodsIterator.name();

                    int httpMethod = model::EndpointNode::HttpMethodFromString(httpMethodStr);

                    if(httpMethod < 0)
                    {
                        qDebug() << "*** Invalid http method " << httpMethodStr << ", skipping entry : " << url;
                        continue;
                    }

					auto entry = new EndpointEntry(this);
					entry->setUrl(url);
                    entry->setHttpMethod(httpMethod);
					auto operationId = jsMethod.property("operationId").toString();
                    if(operationId.toLower() == "undefined")
                    {
                        operationId = "";
                    }
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
                                //TODO: implement support for body and formData param location
                                qDebug() << __FUNCTION__ << " unsupported param location : " << location;
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
