#pragma once
#include "../coremodel_global.h"
#include "IdentifiableEntity.h"
#include <QObject>
#include <QMap>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJSValueIterator>


namespace model
{
	class Project;
	class EndpointEntry;

	class COREMODEL_EXPORT Document : public IdentifiableEntity
	{
		Q_OBJECT
	public:
		Document(Project* project);

		QMap<QString, EndpointEntry*> getEndpointsMap() const;
		QList<EndpointEntry*> getEndpoints() const;

		QJSValue saveToJSValue(PersistenceHandler* handler) const override;
		bool loadFromJSValue(const QJSValue& v);

		bool importFromSwagger(const QString& path);
	};
}
//Q_DECLARE_METATYPE(model::Document*);
