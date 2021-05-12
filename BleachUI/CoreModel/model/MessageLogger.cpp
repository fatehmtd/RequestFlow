#include "MessageLogger.h"

model::MessageLogger::MessageLogger(model::Graph *parent)
	: QObject(parent)
{
	connect(parent, &Graph::started, this, &MessageLogger::clear, Qt::ConnectionType::QueuedConnection);
}

model::MessageLogger::~MessageLogger()
{
}

void model::MessageLogger::log(const Message& message)
{
	_messagesList << message;
	emit logged(message);
	emit updated();
}

#include "IdentifiableEntity.h"

void model::MessageLogger::log(int type, const QString& message, IdentifiableEntity* sender)
{
    //Message m = {type, message, QDateTime::currentDateTime(), sender};
    Message m;
    m.type = type;
    m.message = message;
    m.dateTime = QDateTime::currentDateTime();
    m.sender = sender;
	log(m);
}

void model::MessageLogger::clear()
{
	_messagesList.clear();
	emit cleared();
}

model::Graph* model::MessageLogger::getGraph() const
{
	return dynamic_cast<Graph*>(parent());
}
