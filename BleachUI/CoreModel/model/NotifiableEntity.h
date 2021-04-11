#pragma once
#include "IdentifiableEntity.h"

namespace model
{
	class COREMODEL_EXPORT NotifiableEntity : public IdentifiableEntity
	{
		Q_OBJECT
	public:
		using IdentifiableEntity::IdentifiableEntity;

		virtual void clear(); // used to clear internal attributes
	protected slots:
		virtual void onGraphStart();
		virtual void onGraphStop();
	};
}

