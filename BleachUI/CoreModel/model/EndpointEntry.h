#pragma once
#include "../coremodel_global.h"
#include <QObject>
#include <QList>
#include <QVariant>
#include "IdentifiableEntity.h"

namespace model
{
	class Document;

	class COREMODEL_EXPORT EndpointEntry : public IdentifiableEntity
	{
		Q_OBJECT
		Q_PROPERTY(QString url MEMBER _url READ getUrl WRITE setUrl)
		Q_PROPERTY(QList<QString> productTypes MEMBER _productTypes READ getProductTypes WRITE setProductTypes)
		Q_PROPERTY(QList<QString> consumptionTypes MEMBER _consumptionTypes READ getConsumptionTypes WRITE setConsumptionTypes)
		Q_PROPERTY(QList<QString> queryParams MEMBER _queryParams READ getQueryParams WRITE setQueryParams)
		Q_PROPERTY(QList<QString> pathParams MEMBER _pathParams READ getPathParams WRITE setPathParams)
		Q_PROPERTY(int httpMethod MEMBER _method READ getHttpMethod WRITE setHttpMethod)
	public:
		EndpointEntry(Document* document);

		void setUrl(const QString& url);
		QString getUrl() const;

		void setProductTypes(const QList<QString>& values);
		QList<QString> getProductTypes() const;

		void setConsumptionTypes(const QList<QString>& values);
		QList<QString> getConsumptionTypes() const;

		void setQueryParams(const QList<QString>& values);
		QList<QString> getQueryParams() const;

		void setPathParams(const QList<QString>& values);
		QList<QString> getPathParams() const;

		enum HttpMethod
		{
			GET,
			POST,
			PUT,
			DEL
		};
		Q_ENUM(HttpMethod);

		void setHttpMethod(int method);
		int getHttpMethod() const;

		QJSValue saveToJSValue(PersistenceHandler* handler) const override;
		bool loadFromJSValue(const QJSValue& v) override;

	private:
		QString _url;
		QList<QString> _productTypes;
		QList<QString> _consumptionTypes;
		QList<QString> _queryParams;
		QList<QString> _pathParams;
		int _method = 0;
	};
}