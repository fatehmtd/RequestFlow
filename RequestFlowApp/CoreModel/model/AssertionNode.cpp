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

#include "customjsengine.h"

void model::AssertionNode::evaluate()
{
    CustomJSEngine engine;

	auto requestMessage = getInputSlot()->getData();

    engine.globalObject().setProperty("Request", engine.toScriptValue(requestMessage.toVariant()));

    QString assertion = QString("(function() { try{\n %1;\n return true;\n } catch(err) {\n var baseLine=5; var thisline = new Error().lineNumber-baseLine; return \"Line \" + thisline + \" -> \" + err; \n} })").arg(_script);

    engine.globalObject().setProperty("assert", engine.evaluate(assertion));
    QJSValue result = engine.evaluate("assert();");

    if (result.isError() || !result.isBool())
    {
		fail(result.toString());
		return;
    }
    else
    {
        getOutputSlot()->setData(requestMessage);
        Node::evaluate();
    }
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
