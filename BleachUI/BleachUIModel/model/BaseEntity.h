#pragma once
#include <QObject>
#include "../bleachuimodel_global.h"

namespace model
{
	class BLEACHUIMODEL_EXPORT BaseEntity : public QObject
	{
		Q_OBJECT
	public:
		BaseEntity(QObject* parent);

		virtual void clear(); // used to clear internal attributes

	protected slots:
		virtual void onGraphStart();
		virtual void onGraphStop();
	};
}

