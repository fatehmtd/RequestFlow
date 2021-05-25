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

		QMap<QString, InputSlot*> getInputSlotsMap() const;
		QMap<QString, OutputSlot*> getOutputSlotsMap() const;

		QList<InputSlot*> getInputSlots() const;
        QList<OutputSlot*> getOutputSlots() const;

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

        virtual void prepareNodeInternals();
	private:
		void setStatus(int status);
	public slots:
		virtual void evaluate();
		virtual void fail(const QString& reason);

		virtual void onGraphStart() override;
		virtual void onGraphStop() override;

        virtual void dataReceived(model::InputSlot* slot);

	signals:
		void evaluated();
		void exceptionRaised(QString reason);
		void ready();
		void failed(const QString&);

	protected slots:
		void slotDataReceived();
		void slotDataSent();
		void slotFailed();

	protected:
		QList<InputSlot*> _listOfReadySlots;
        int _executionStatus=0;
	};
}
