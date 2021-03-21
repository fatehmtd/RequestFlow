#include "DelayNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Slot.h>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

logic::DelayNode::DelayNode(model::Node* modelNode) : view::Node(modelNode)
{
	setupUi();
	setTitle("Delay");
}

void logic::DelayNode::clearUI()
{
	//_editor->clear();
}

void logic::DelayNode::onTimeout()
{
	_node->getOutputSlots().first()->setData(_node->getInputSlots().first()->getData());
	_node->evaluate();
}

void logic::DelayNode::setupUi()
{
	auto widget = new QWidget();
	_ui.setupUi(widget);

	getContentWidget()->layout()->addWidget(widget);

	setResizable(false);

	setMinSize(QSize(50, 50));

	_timer = new QTimer(this);
	_timer->setSingleShot(true);
	connect(_timer, &QTimer::timeout, this, &DelayNode::onTimeout);

	_bgColor = view::colors::yellow;
	//itle->setDefaultTextColor(view::colors::darkGrey);

	connect(_node, &model::Node::ready, this, [=]()
		{
			_timer->start(_ui.spinBox->value());
		});

	setSize(100, 50);
}
