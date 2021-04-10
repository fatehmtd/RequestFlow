#pragma once
#include <QObject>
#include "../coremodel_global.h"

namespace model
{
	class COREMODEL_EXPORT BaseEntity : public QObject
	{
		Q_OBJECT
	public:
		BaseEntity(QObject* parent);

		virtual void clear(); // used to clear internal attributes

		void setIdentifier(QString id);
		void setType(QString type);

		QString getIdentifier() const;
		QString getType() const;

	protected slots:
		virtual void onGraphStart();
		virtual void onGraphStop();
	private:
		QString _id, _type;
	};
}

