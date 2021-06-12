#pragma once
#include "NotifiableEntity.h"
#include <QMap>

namespace model
{
	class Graph;
	class InputSlot;
	class OutputSlot;
    class GroupNode;

	class COREMODEL_EXPORT Node : public NotifiableEntity
	{
		Q_OBJECT
	public:
        Node(Graph* graph, const QString& typeName);

        // Slot query
		InputSlot* getDestinationSlot(const QString& name) const;
		OutputSlot* getOriginSlot(const QString& name) const;

		InputSlot* getDestinationSlotByIdentifier(const QString& identifier) const;
		OutputSlot* getOriginSlotByIdentifier(const QString& identifier) const;

        // Slot management
		InputSlot* addInputSlot(QString name, int dataType);
		OutputSlot* addOutputSlot(QString name, int dataType);

        virtual QMap<QString, InputSlot*> getInputSlotsMap() const;
        virtual QMap<QString, OutputSlot*> getOutputSlotsMap() const;

        virtual QList<InputSlot*> getInputSlots() const;
        virtual QList<OutputSlot*> getOutputSlots() const;

        // Misc
		Graph* getGraph() const;

		virtual void clear() override;
        virtual void createModel() {};
        virtual void prepareNodeInternals();

        // Execution
        enum Status
		{
			IDLE,
			READY,
			EVALUATED,
			FAILED
		};

		int getStatus() const;

        // Group management
        GroupNode *getGroup() const;

        // Persistence
        QJSValue saveToJSValue(PersistenceHandler* persistenceHandler) const override;
        bool loadFromJSValue(const QJSValue& v) override;
    protected:
        void setGroup(GroupNode* group);

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
        GroupNode* _group = nullptr;
	};
}
