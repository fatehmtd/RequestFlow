#pragma once
#include <QObject>
#include "../bleachuimodel_global.h"

namespace model
{
	class Node;
	class Edge;

	class BLEACHUIMODEL_EXPORT Graph : public QObject
	{
		Q_OBJECT
	public:
		Graph();
		~Graph();

		QList<Node*> getNodes() const;
		QList<Edge*> getEdges() const;

		virtual void start();
		virtual void stop();

	signals:
		void started();
		void stopped();
	};
}