#pragma once
#include "../coremodel_global.h"
#include <QObject>
#include <QList>
#include <QStringList>
#include <QVariant>
#include "IdentifiableEntity.h"

namespace model
{
	class Document;

	class COREMODEL_EXPORT EndpointEntry : public IdentifiableEntity
	{
		Q_OBJECT
		Q_PROPERTY(QString url MEMBER _url READ getUrl WRITE setUrl)
		Q_PROPERTY(QStringList productTypes MEMBER _productTypes READ getProductTypes WRITE setProductTypes)
		Q_PROPERTY(QStringList consumptionTypes MEMBER _consumptionTypes READ getConsumptionTypes WRITE setConsumptionTypes)
		Q_PROPERTY(QStringList queryParams MEMBER _queryParams READ getQueryParams WRITE setQueryParams)
		Q_PROPERTY(QStringList pathParams MEMBER _pathParams READ getPathParams WRITE setPathParams)
		Q_PROPERTY(int httpMethod MEMBER _method READ getHttpMethod WRITE setHttpMethod)
	public:
		EndpointEntry(Document* document);

		void setUrl(const QString& url);
		QString getUrl() const;

		void setProductTypes(const QStringList& values);
		QStringList getProductTypes() const;

		void setConsumptionTypes(const QStringList& values);
		QStringList getConsumptionTypes() const;

		void setQueryParams(const QStringList& values);
		QStringList getQueryParams() const;

		void setPathParams(const QStringList& values);
		QStringList getPathParams() const;

		enum HttpMethod
		{
			GET,
			POST,
			PUT,
            DEL,
            PATCH
		};
		Q_ENUM(HttpMethod);

		void setHttpMethod(int method);
		int getHttpMethod() const;

	private:
		QString _url;
		QStringList _productTypes;
		QStringList _consumptionTypes;
		QStringList _queryParams;
		QStringList _pathParams;
		int _method = 0;
	};
}

//Q_DECLARE_METATYPE(model::EndpointEntry*);
