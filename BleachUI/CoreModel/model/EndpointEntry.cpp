#include "EndpointEntry.h"
#include "Document.h"

model::EndpointEntry::EndpointEntry(Document* document) : IdentifiableEntity(document)
{
	setType("EndpointEntry");
}

void model::EndpointEntry::setUrl(const QString& url)
{
	_url = url;
}

QString model::EndpointEntry::getUrl() const
{
	return _url;
}

void model::EndpointEntry::setProductTypes(const QStringList& values)
{
	_productTypes = values;
}

QStringList model::EndpointEntry::getProductTypes() const
{
	return _productTypes;
}

void model::EndpointEntry::setConsumptionTypes(const QStringList& values)
{
	_consumptionTypes = values;
}

QStringList model::EndpointEntry::getConsumptionTypes() const
{
	return _consumptionTypes;
}

void model::EndpointEntry::setQueryParams(const QStringList& values)
{
	_queryParams = values;
}

QStringList model::EndpointEntry::getQueryParams() const
{
	return _queryParams;
}

void model::EndpointEntry::setPathParams(const QStringList& values)
{
	_pathParams = values;
}

QStringList model::EndpointEntry::getPathParams() const
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