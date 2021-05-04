#include "AssertionNode.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJSEngine>
#include <QJSValue>
#include <QFile>
#include <QTextStream>

void model::AssertionNode::createModel()
{
	addInputSlot("Input", Slot::CUSTOM);
	addOutputSlot("Output", Slot::CUSTOM);
}

void model::AssertionNode::setScript(const QString& script)
{
	_script = script;
}

QString model::AssertionNode::getScript() const
{
	return _script;
}

void model::AssertionNode::evaluate()
{
	QJSEngine engine;
	engine.installExtensions(QJSEngine::Extension::AllExtensions);

	{
		QFile fp(":/js/jsonpath");
		if (fp.open(QIODevice::ReadOnly))
		{
			QTextStream data(&fp);
			auto jsonPath = engine.evaluate(data.readAll());
			auto pathOfFunction = engine.evaluate("(function (path, obj) { return JSONPath.JSONPath(path, obj);})");
			engine.globalObject().setProperty("pathOf", pathOfFunction);
		}
	}

	auto requestMessage = getInputSlot()->getData();

	engine.globalObject().setProperty("Request", engine.toScriptValue(requestMessage.toVariant()));
	//engine.globalObject().setProperty("Response", engine.toScriptValue(response.toVariant()));

	QJSValue result = engine.evaluate(_script);

	if (result.isError())
	{
		qDebug() << result.toString();
		fail(result.toString());
		return;
	}
	else if (!result.toBool())
	{
		qDebug() << result.toString();
		fail(result.toString());
		return;
	}

	getOutputSlot()->setData(requestMessage);
	Node::evaluate();
}

model::InputSlot* model::AssertionNode::getInputSlot() const
{
	return getInputSlotsMap().values()[0];
}

model::OutputSlot* model::AssertionNode::getOutputSlot() const
{
	return getOutputSlotsMap().values()[0];
}

model::AssertionNode::AssertionNode(model::Graph* graph) : Node(graph, "Assertion")
{
	setScript("");
}