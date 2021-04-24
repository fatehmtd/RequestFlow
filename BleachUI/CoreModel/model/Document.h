#pragma once
#include "../coremodel_global.h"
#include "IdentifiableEntity.h"
#include <QObject>
#include <QMap>
#include <QList>

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
	};
}
