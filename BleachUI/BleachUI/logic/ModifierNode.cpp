#include "ModifierNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Slot.h>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

logic::ModifierNode::ModifierNode(model::Node* modelNode) : view::Node(modelNode), _ui(new Ui::ModifierNodeUiForm)
{
	setupUi();
	setTitle("Modifier");
}

void logic::ModifierNode::clearUI()
{
	//_editor->clear();
}

void logic::ModifierNode::setupUi()
{
	auto widget = new QWidget();
	_ui->setupUi(widget);
	getContentWidget()->layout()->addWidget(widget);
	//auto outputSlot = new view::Slot(this, false);
	_bgColor = view::colors::queenBlue;
	connect(_node, &model::Node::ready, this, [=]()
		{
			_node->getOutputSlots().first()->setData(_node->getInputSlots().first()->getData());
			_node->evaluate();
		});

	setMinSize(QSize(450, 300));
	setSize(400, 400);	
}
