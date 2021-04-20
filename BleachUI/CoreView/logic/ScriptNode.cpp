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
	//_editor->clear();
}

#include <QTextStream>

void logic::ScriptNode::setupUi()
{
	QString buffer;
	QTextStream output(&buffer);

	output << "Response.body = Request.body; // the body of the request" << "\n";
	output << "Response.query = Request.query; // the query parameters ex : ?key=value" << "\n";
	output << "Response.path = Request.path; // the path variables ex: http://website/customers/:id" << "\n";
	output << "Response.context = Request.context; // the current execution context, contains anything useful" << "\n";
	output << "Response.body = {\"firstName\" : \"James\", \"lastName\" : \"jamon\"};" << "\n";

	_editor = new QTextEdit();
	_editor->setText(buffer);
	_editor->setPlaceholderText("// javascript");
	getContentWidget()->layout()->addWidget(_editor);

	//_bgColor = view::colors::charcoal;
	_bgColor = view::colors::queenBlue;

	connect(_node, &model::Node::ready, this, [=]()
		{
			dynamic_cast<model::ScriptNode*>(getModelNode())->setScript(_editor->toPlainText());
			getModelNode()->evaluate();
		});

	setMinSize(QSize(500, 500));
	setSize(300, 200);
}