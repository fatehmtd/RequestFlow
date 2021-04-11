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
