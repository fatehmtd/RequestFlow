#include "PayloadNode.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

model::PayloadNode::PayloadNode(model::Graph* graph) : model::Node(graph, "Payload")
{

}

void model::PayloadNode::createModel()
{
	addOutputSlot("output", Slot::CUSTOM);
}

void model::PayloadNode::setMessage(const model::Message& message)
{
    _message = message;
}

void model::PayloadNode::setFilePath(const QString &path)
{
    _filePath = path;
}

model::Message model::PayloadNode::getMessage() const
{
	return _message;
}

void model::PayloadNode::evaluate()
{
	getOutputSlot()->setData(getMessage());
	Node::evaluate();
}

model::OutputSlot* model::PayloadNode::getOutputSlot() const
{
    return getOutputSlotsMap().values()[0];
}

bool model::PayloadNode::getLoadFromFile() const
{
    return _loadFromFileStatus;
}

QString model::PayloadNode::getFilePath() const
{
    return _filePath;
}

#include <QFile>
#include <QDir>
#include <QTextStream>

model::Message model::PayloadNode::loadMessageFromFile(const QString &path) const
{
    model::Message message;
    QFile fp(path);
    if(fp.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream stream(&fp);
        auto data = stream.readAll();
        auto temp = QJsonDocument::fromJson(data.toLocal8Bit()).toVariant();
        message.fromVariant(temp);
    }
    return message;
}

void model::PayloadNode::setLoadFromFile(bool status)
{
    _loadFromFileStatus = status;
}

QVariant model::PayloadNode::messageToVariant() const
{
	return _message.toVariant();
}

void model::PayloadNode::messageFromVariant(const QVariant& v)
{
	_message.fromVariant(v);
}
