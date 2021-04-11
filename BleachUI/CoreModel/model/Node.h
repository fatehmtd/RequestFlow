#pragma once
#include "NotifiableEntity.h"
#include <QMap>

namespace model
{
	class Graph;
	class InputSlot;
	class OutputSlot;

	class COREMODEL_EXPORT Node : public NotifiableEntity
	{
		Q_OBJECT
	public:
		Node(Graph* parent, const QString& typeName);

		InputSlot* getDestinationSlot(const QString& name) const;
		OutputSlot* getOriginSlot(const QString& name) const;

		InputSlot* addInputSlot(QString name, int dataType);
		OutputSlot* addOutputSlot(QString name, int dataType);

		QMap<QString, InputSlot*> getInputSlots() const;
		QMap<QString, OutputSlot*> getOutputSlots() const;

		Graph* getGraph() const;

		virtual void clear() override;

		virtual void createModel() {};

	public slots:
		virtual void evaluate();
		void raiseException(QString reason);

		virtual void onGraphStart() override;
		virtual void onGraphStop() override;

		virtual void dataReceived(InputSlot* slot);

	signals:
		void evaluated();
		void exceptionRaised(QString reason);
		void ready();

	protected slots:
		void slotDataReceived();
		void slotDataSent();

	protected:
		QList<InputSlot*> _listOfReadySlots;
	};
}