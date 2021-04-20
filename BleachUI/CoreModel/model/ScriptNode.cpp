#include "ScriptNode.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJSEngine>
#include <QJSValue>

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
	return getInputSlots().values()[0];
}

model::OutputSlot* model::ScriptNode::getOutputSlot() const
{
	return getOutputSlots().values()[0];
}

void model::ScriptNode::createModel()
{
	addInputSlot("Input", InputSlot::CUSTOM);
	addOutputSlot("Output", InputSlot::CUSTOM);
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

	auto requestMessage = getInputSlot()->getData();

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
	getOutputSlot()->setData(response);
	return true;
}
