#pragma once
#include <QObject>
#include <QList>
#include "../bleachuimodel_global.h"

namespace model
{
	class Node;
	class Edge;
	class OutputSlot;
	class InputSlot;

	class BLEACHUIMODEL_EXPORT Graph : public QObject
	{
		Q_OBJECT
	public:
		Graph();
		~Graph();

		QList<Node*> getNodes() const;
		QList<Edge*> getEdges() const;

		Edge* connectSlots(OutputSlot* origin, InputSlot* destination);

		enum Status
		{
			OK,
			MISSING_STARTING_NODE,
			MISSING_ENDING_NODE,
			MISSING_EDGES
		};

	public slots:
		virtual int start();
		virtual void stop();
		virtual void onNodeEvaluated();	
	signals:
		void started();
		void stopped();
	private:
		int computeExecutionPath();
		void clear();
	private:
		QList<Node*> _startingNodes, _endingNodes;
	};
}