#include "ScriptNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Slot.h>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJSEngine>
#include <QJSValue>

logic::ScriptNode::ScriptNode(model::Node* modelNode) : view::Node(modelNode, "Script")
{
	setupUi();
	setTitle("Script");
}

QJSValue logic::ScriptNode::toJSValue(QJSEngine& engine) const
{
	auto value = Node::toJSValue(engine);
	value.setProperty("_script", _editor->toPlainText());
	return value;
}

void logic::ScriptNode::fromJSValue(const QJSValue& jsValue)
{
	Node::fromJSValue(jsValue);
	_editor->setText(jsValue.property("_script").toString());
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

	auto outputSlot = _node->getOutputSlots().first();

	//_bgColor = view::colors::charcoal;
	_bgColor = view::colors::queenBlue;

	connect(_node, &model::Node::ready, this, [=]()
		{
			executeScript();
		});

	setMinSize(QSize(500, 500));
	setSize(300, 200);
}

void logic::ScriptNode::executeScript()
{
	QJSEngine engine;

	auto inputSlot = _node->getInputSlots().first();
	auto outputSlot = _node->getOutputSlots().first();

	auto requestMessage = inputSlot->getData();

	model::Message response;

	auto source = _editor->toPlainText();

	engine.globalObject().setProperty("Request", engine.toScriptValue(requestMessage.toVariant()));
	engine.globalObject().setProperty("Response", engine.toScriptValue(response.toVariant()));

	QJSValue result = engine.evaluate(source);

	if (result.isError())
	{
		qDebug() << result.toString();
		_node->raiseException(result.toString());
	}
	else
	{
		auto value = engine.fromScriptValue<QVariant>(engine.evaluate("Response"));
		response.fromVariant(value);

		outputSlot->setData(response);
		_node->evaluate();
	}
}
