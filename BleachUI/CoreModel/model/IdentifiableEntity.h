#pragma once
#include "../coremodel_global.h"
#include <QObject>
#include "PersistableEntity.h"

namespace model
{
	class COREMODEL_EXPORT IdentifiableEntity : public PersistableEntity
	{
		Q_OBJECT
		Q_PROPERTY(QString identifier READ getIdentifier WRITE setIdentifier NOTIFY identifierChanged)
		Q_PROPERTY(QString name MEMBER _name READ getName WRITE setName NOTIFY nameChanged)
		Q_PROPERTY(QString entityType MEMBER _type READ getType WRITE setType)
	public:
		IdentifiableEntity(QObject* parent);

		void setIdentifier(QString id);
		QString getIdentifier() const;

		void setType(QString type);
		QString getType() const;

		void setName(QString name);
		QString getName() const;

	signals:
		void identifierChanged(const QString&);
		void nameChanged(const QString&);
	private:
		QString _name, _type;
	};
}