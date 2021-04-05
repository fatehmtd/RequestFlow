#pragma once
#include "BaseEntity.h"
#include <QMap>

namespace model
{
	class Graph;
	class InputSlot;
	class OutputSlot;

	class COREMODEL_EXPORT Node : public BaseEntity
	{
		Q_OBJECT
	public:
		Node(Graph* parent, const QString& name);

		InputSlot* getDestination(const QString& name) const;
		OutputSlot* getOrigin(const QString& name) const;

		InputSlot* addInputSlot(QString name, int dataType);
		OutputSlot* addOutputSlot(QString name, int dataType);

		QMap<QString, InputSlot*> getInputSlots() const;
		QMap<QString, OutputSlot*> getOutputSlots() const;

		Graph* getGraph() const;

		virtual void clear() override;

	public slots:
		virtual void evaluate();
		void raiseException(QString reason);

	protected:		
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