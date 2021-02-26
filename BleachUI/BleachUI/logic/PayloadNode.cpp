#include "PayloadNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Slot.h>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

logic::PayloadNode::PayloadNode(model::Node* modelNode) : view::Node(modelNode)
{
	setupUi();
	setTitle("Payload");
}

void logic::PayloadNode::setupUi()
{
	QJsonObject json =
	{
		{"firstName", "John"},
		{"lastName", "Do"}
	};
	
	_editor = new QTextEdit();
	_editor->setText(QJsonDocument(json).toJson(QJsonDocument::JsonFormat::Compact));
	getContentWidget()->layout()->addWidget(_editor);
	//auto outputSlot = new view::Slot(this, false);
	_bgColor = view::colors::byzantium;
	connect(_node, &model::Node::ready, this, [=]()
		{
			_node->getOutputSlots().first()->setData(_editor->toPlainText());
			_node->evaluate();
		});

	setSize(300, 200);
}
