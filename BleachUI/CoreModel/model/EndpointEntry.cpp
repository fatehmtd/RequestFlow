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

template <typename T>
QJSValue saveListtoJSValue(const QList<T>& list, model::PersistenceHandler* handler)
{
	auto jsArray = handler->createJsValueArray(list.size());
	for (int i = 0; i < list.size(); i++)
	{
		jsArray.setProperty(i, handler->createJsValue(list[i]));
	}	
	return jsArray;
}

template <typename T>
QList<T> loadListFromJSValue(const QString& name, const QJSValue& v)
{
	QList<T> output;
	auto jsArray = v.property(name);
	for (int i = 0; i < jsArray.property("length").toInt(); i++)
	{
		output << jsArray.property(i).toString();
	}
	return output;
}

QJSValue model::EndpointEntry::saveToJSValue(PersistenceHandler* handler) const
{
	auto value = PersistableEntity::saveToJSValue(handler);
	value.setProperty("queryParams", saveListtoJSValue(getQueryParams(), handler));
	value.setProperty("pathParams", saveListtoJSValue(getPathParams(), handler));
	value.setProperty("productTypes", saveListtoJSValue(getProductTypes(), handler));
	value.setProperty("consumptionTypes", saveListtoJSValue(getConsumptionTypes(), handler));
	return value;
}

bool model::EndpointEntry::loadFromJSValue(const QJSValue& v)
{
	PersistableEntity::loadFromJSValue(v);
	setQueryParams(loadListFromJSValue<QString>("queryParams", v));
	setPathParams(loadListFromJSValue<QString>("pathParams", v));
	setProductTypes(loadListFromJSValue<QString>("productTypes", v));
	setConsumptionTypes(loadListFromJSValue<QString>("consumptionTypes", v));
	return true;
}
