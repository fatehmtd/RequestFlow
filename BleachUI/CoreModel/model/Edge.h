#pragma once
#include "../coremodel_global.h"
#include "BaseEntity.h"

namespace model
{
	class InputSlot;
	class OutputSlot;
	class Graph;

	class COREMODEL_EXPORT Edge : public BaseEntity
	{
		Q_OBJECT
	public:
		Edge(Graph* parent);

		bool setDestination(InputSlot* slot);
		bool setOrigin(OutputSlot* slot);

		InputSlot* getDestinationSlot() const;
		OutputSlot* getOriginSlot() const;
	protected:
		InputSlot* _destinationSlot = nullptr;
		OutputSlot* _originSlot = nullptr;

	signals:
		void dataReceived();

	protected slots:
		void onDataReceived();
	};
}