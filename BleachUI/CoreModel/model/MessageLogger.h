#pragma once

#include <QObject>
#include <QDebug>

#include "Node.h"
#include "Graph.h"
#include <QDateTime>

namespace model
{
	class IdentifiableEntity;
	class COREMODEL_EXPORT MessageLogger : public QObject
	{
		Q_OBJECT

	public:
		MessageLogger(Graph* parent);
		~MessageLogger();

		enum MessageType
		{
			SUCCESS,
			INFO,
			WARNING,
			DEBUG,
			ERROR
		};

		struct Message
		{
			int type = MessageType::INFO;
			QString message = "";
			QDateTime dateTime;
			IdentifiableEntity* sender= nullptr;
		};

		void log(const Message& message);
		void log(int type, const QString& message, IdentifiableEntity* sender);
		void clear();

		Graph* getGraph() const;
	signals:
		void updated();
		void cleared();
		void logged(const MessageLogger::Message&);
	private:
		QList<Message> _messagesList;
	};
}