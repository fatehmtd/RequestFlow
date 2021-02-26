#include "ViewerNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Slot.h>
#include <QDebug>

logic::ViewerNode::ViewerNode(model::Node* modelNode) : view::Node(modelNode)
{
	setupUi();
	setTitle("Viewer");
}

void logic::ViewerNode::setupUi()
{
	_editor = new QTextEdit();
	getContentWidget()->layout()->addWidget(_editor);
	_bgColor = view::colors::green;
	connect(_node, &model::Node::ready, this, [=]()
		{
			_editor->setPlainText(_node->getInputSlots().first()->getData().toString().toUtf8());
			//qDebug() << _editor->toPlainText();
			_node->evaluate();
		});

	setSize(300, 200);
}
