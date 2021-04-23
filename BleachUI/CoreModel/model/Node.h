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

		InputSlot* getDestinationSlotByIdentifier(const QString& identifier) const;
		OutputSlot* getOriginSlotByIdentifier(const QString& identifier) const;

		InputSlot* addInputSlot(QString name, int dataType);
		OutputSlot* addOutputSlot(QString name, int dataType);

		QMap<QString, InputSlot*> getInputSlots() const;
		QMap<QString, OutputSlot*> getOutputSlots() const;

		Graph* getGraph() const;

		virtual void clear() override;

		virtual void createModel() {};

		QJSValue saveToJSValue(PersistenceHandler* persistenceHandler) const override;
		bool loadFromJSValue(const QJSValue& v) override;

		enum Status
		{
			IDLE,
			READY,
			EVALUATED,
			FAILED
		};

		int getStatus() const;
	private:
		void setStatus(int status);
	public slots:
		virtual void evaluate();
		virtual void fail();
		void raiseException(QString reason);

		virtual void onGraphStart() override;
		virtual void onGraphStop() override;

		virtual void dataReceived(InputSlot* slot);

	signals:
		void evaluated();
		void exceptionRaised(QString reason);
		void ready();
		void failed();

	protected slots:
		void slotDataReceived();
		void slotDataSent();
		void slotFailed();

	protected:
		QList<InputSlot*> _listOfReadySlots;
		int _executionStatus=0;
	};
}