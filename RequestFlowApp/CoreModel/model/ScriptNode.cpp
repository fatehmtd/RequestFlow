#include "ScriptNode.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJSEngine>
#include <QJSValue>

#include <QTextStream>

model::ScriptNode::ScriptNode(model::Graph* graph) : Node(graph, "Script")
{
	QString buffer;
	QTextStream output(&buffer);

	output << "Response.body = Request.body; // the body of the request" << "\n";
	output << "Response.query = Request.query; // the query parameters ex : ?key=value" << "\n";
	output << "Response.path = Request.path; // the path variables ex: http://website/customers/:id" << "\n";
	output << "Response.context = Request.context; // the current execution context, contains anything useful" << "\n";
	//output << "Response.body = {\"firstName\" : \"James\", \"lastName\" : \"jamon\"};" << "\n";

    //connect(this, &ScriptNode::ready, this, &ScriptNode::evaluate);

	setScript(buffer);
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
	return getInputSlotsMap().values()[0];
}

model::OutputSlot* model::ScriptNode::getOutputSlot() const
{
	return getOutputSlotsMap().values()[0];
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

#include <QFile>
#include <QTextStream>

bool model::ScriptNode::executeScript()
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

	model::Message response;

	auto inputSlots = getInputSlots();
	if (!inputSlots.isEmpty())
	{
		if (inputSlots.size() >= 1)
		{
			int index = 0;
			for (auto slot : inputSlots)
			{
				auto requestMessage = slot->getData();
				engine.globalObject().setProperty(QString("Request%1").arg(index++), engine.toScriptValue(requestMessage.toVariant()));
			}

			auto requestMessage = getInputSlot()->getData();
			engine.globalObject().setProperty("Request", engine.toScriptValue(requestMessage.toVariant()));
		}
	}
	engine.globalObject().setProperty("Response", engine.toScriptValue(response.toVariant()));

	QJSValue result = engine.evaluate(_script);

	if (result.isError())
	{
		fail(result.toString());
		return false;
	}

	auto value = engine.fromScriptValue<QVariant>(engine.evaluate("Response"));
	response.fromVariant(value);
	getOutputSlot()->setData(response);
	return true;
}
