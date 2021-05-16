#include "DelayNode.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

model::DelayNode::DelayNode(model::Graph* graph) : model::Node(graph, "Delay")
{
	_timer.setInterval(1000);
	_timer.setSingleShot(true);
	connect(&_timer, &QTimer::timeout, this, &DelayNode::onTimeout, Qt::ConnectionType::DirectConnection);

	connect(this, &Node::ready, this, [=]()
		{
			_timer.start();
		}, Qt::ConnectionType::DirectConnection);

    connect(graph, &Graph::stopped, this, [=]()
    {
        _timer.stop();
    });
}

void model::DelayNode::createModel()
{
	addInputSlot("input", Slot::DataType::CUSTOM);
	addOutputSlot("output", Slot::DataType::CUSTOM);
}

void model::DelayNode::setDelay(int duration)
{
	_timer.setInterval(duration);
}

int model::DelayNode::getDelay() const
{
	return _timer.interval();
}

void model::DelayNode::onTimeout()
{
	getOutputSlotsMap().values()[0]->setData(getInputSlotsMap().values()[0]->getData());
	evaluate();
}
