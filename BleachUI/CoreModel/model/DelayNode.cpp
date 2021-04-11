#include "DelayNode.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

model::DelayNode::DelayNode(model::Graph* graph) : model::Node(graph, "Delay")
{
	_timer.setInterval(1000);
	_timer.setSingleShot(true);
	connect(&_timer, &QTimer::timeout, this, &DelayNode::onTimeout);

	connect(this, &Node::ready, this, [=]()
		{
			qDebug() << "ready";
			_timer.start();
		});
}

void model::DelayNode::createModel()
{
	_inputSlot = addInputSlot("input", Slot::DataType::CUSTOM);
	_outputSlot = addOutputSlot("output", Slot::DataType::CUSTOM);
}

void model::DelayNode::setDelay(int duration)
{
	_timer.setInterval(duration);
}

unsigned int model::DelayNode::getDelay() const
{
	return _timer.interval();
}

void model::DelayNode::onTimeout()
{
	_outputSlot->setData(_inputSlot->getData());
	evaluate();
}
