#pragma once
#include <QObject>
#include <QList>
#include <QMap>
#include <QVariant>
#include "IdentifiableEntity.h"

namespace model
{
	class Node;
	class Edge;
	class Slot;
	class OutputSlot;
	class InputSlot;
	class Environment;
	class Project;

	class COREMODEL_EXPORT Graph : public IdentifiableEntity
	{
		Q_OBJECT
	public:
		Graph(Project* parent);
		~Graph();

		Project* getProject() const;

		QList<Node*> getNodes() const;
		QList<Edge*> getEdges() const;

		Edge* findEdge(const InputSlot* destination, const OutputSlot* origin) const;
		QList<Edge*> findEdges(const Slot* slot) const;
		QList<Edge*> findEdges(const Node* node) const;

		Edge* connectSlots(OutputSlot* origin, InputSlot* destination);
		bool canConnectSlots(Slot* origin, Slot* destination) const;

		enum ErrorCode
		{
			OK,
			MISSING_STARTING_NODE,
			MISSING_ENDING_NODE,
			MISSING_EDGES
		};

		void setActiveEnvironment(Environment* env);
		Environment* getActiveEnvironment() const;

		QJSValue saveToJSValue(PersistenceHandler* persistenceHandler) const override;
		bool loadFromJSValue(const QJSValue& v) override;

	public slots:
		virtual int start();
		virtual void stop();
		virtual void onNodeEvaluated();	
		virtual void onNodeFailed();	
		virtual void onNodeException(QString reason);
	signals:
		void started();
		void stopped();
		void exceptionRaised(Node* node, QString reason);
	private:
		int computeExecutionPath();
		void clear();
		void checkExecutionStatus();
	private:
		QList<Node*> _startingNodes, _endingNodes;
		QMap<Node*, int> _executionNodes;
		Environment* _environment = nullptr;
	};
}