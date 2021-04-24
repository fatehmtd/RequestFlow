#include "EndpointEntry.h"
#include "Document.h"

model::EndpointEntry::EndpointEntry(Document* document) : IdentifiableEntity(document)
{

}

void model::EndpointEntry::setUrl(const QString& url)
{
	_url = url;
}

QString model::EndpointEntry::getUrl() const
{
	return _url;
}

void model::EndpointEntry::setProductTypes(const QList<QString>& values)
{
	_productTypes = values;
}

QList<QString> model::EndpointEntry::getProductTypes() const
{
	return _productTypes;
}

void model::EndpointEntry::setConsumptionTypes(const QList<QString>& values)
{
	_consumptionTypes = values;
}

QList<QString> model::EndpointEntry::getConsumptionTypes() const
{
	return _consumptionTypes;
}

void model::EndpointEntry::setQueryParams(const QList<QString>& values)
{
	_queryParams = values;
}

QList<QString> model::EndpointEntry::getQueryParams() const
{
	return _queryParams;
}

void model::EndpointEntry::setPathParams(const QList<QString>& values)
{
	_pathParams = values;
}

QList<QString> model::EndpointEntry::getPathParams() const
{
	return _pathParams;
}

void model::EndpointEntry::setHttpMethod(int method)
{
	_method = method;
}

int model::EndpointEntry::getHttpMethod() const
{
	return _method;
}
