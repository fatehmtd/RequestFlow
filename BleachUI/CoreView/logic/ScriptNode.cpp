#include "ScriptNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Slot.h>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJSEngine>
#include <QJSValue>

logic::ScriptNode::ScriptNode(model::ScriptNode* modelNode) : view::Node(modelNode, "Script")
{
	setupUi();
	setTitle("Script");
}

void logic::ScriptNode::clearUI()
{
}

#include <QTextStream>

void logic::ScriptNode::setupUi()
{
	auto node = getModelNode<model::ScriptNode*>();
	_editor = new QPlainTextEdit();
	_editor->setPlainText(node->getScript());
	_editor->setPlaceholderText("// javascript");
	getContentWidget()->layout()->addWidget(_editor);

	connect(_editor, &QPlainTextEdit::textChanged, this, [=]()
		{
			node->setScript(_editor->toPlainText());
		});

	_bgColor = view::colors::charcoal;
	//_bgColor = view::colors::queenBlue;

	connect(_node, &model::Node::ready, this, [=]()
		{
			getModelNode()->evaluate();
		}, Qt::ConnectionType::QueuedConnection);

	setMinSize(QSize(300, 300));
	setSize(300, 200);
}