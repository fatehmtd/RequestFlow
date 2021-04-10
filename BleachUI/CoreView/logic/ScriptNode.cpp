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

/// <summary>
/// 
/// </summary>
/// <param name="graph"></param>
/// 


model::ScriptNode::ScriptNode(model::Graph* graph) : Node(graph, "Script")
{

}

void model::ScriptNode::setScript(const QString& script)
{
	_script = script;
}

QString model::ScriptNode::getScript() const
{
	return _script;
}

model::InputSlot* model::ScriptNode::getInputSlot() const
{
	return _inputSlot;
}

model::OutputSlot* model::ScriptNode::getOutputSlot() const
{
	return _outputSlot;
}

void model::ScriptNode::createModel()
{
	_inputSlot = addInputSlot("Input", InputSlot::CUSTOM);
	_outputSlot = addOutputSlot("Output", InputSlot::CUSTOM);
}

void model::ScriptNode::evaluate()
{
	if (executeScript())
	{
		Node::evaluate();
	}
}

bool model::ScriptNode::executeScript()
{
	QJSEngine engine;
	model::Message response;

	auto requestMessage = _inputSlot->getData();

	engine.globalObject().setProperty("Request", engine.toScriptValue(requestMessage.toVariant()));
	engine.globalObject().setProperty("Response", engine.toScriptValue(response.toVariant()));

	QJSValue result = engine.evaluate(_script);

	if (result.isError())
	{
		qDebug() << result.toString();
		raiseException(result.toString());
		return false;
	}

	auto value = engine.fromScriptValue<QVariant>(engine.evaluate("Response"));
	response.fromVariant(value);
	_outputSlot->setData(response);
	return true;
}
