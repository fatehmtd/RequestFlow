#include "PayloadNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Slot.h>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTableWidgetItem>

logic::PayloadNode::PayloadNode(model::PayloadNode* modelNode) : view::Node(modelNode, "Payload")
{
	setupUi();
	setTitle("Payload");
}

QJSValue logic::PayloadNode::toJSValue(QJSEngine& engine) const
{
	auto value = Node::toJSValue(engine);
	auto message = composeMessage().toVariant();
	value.setProperty("_message", engine.toScriptValue<QVariant>(message));
	return value;
}

void logic::PayloadNode::fromJSValue(const QJSValue& jsValue)
{
	Node::fromJSValue(jsValue);
	auto variant = jsValue.toVariant();
}

model::Message logic::PayloadNode::composeMessage() const
{
	model::Message message(_ui.textEdit_body->toPlainText());
	message.setPathVars(fillFromTable(_ui.tableWidget_path));
	message.setQueryParams(fillFromTable(_ui.tableWidget_query));
	return message;
}

void logic::PayloadNode::clearUI()
{

}

void logic::PayloadNode::setupUi()
{
	_bgColor = view::colors::byzantium;

	auto widget = new QWidget();
	_ui.setupUi(widget);

	_ui.tableWidget_path->setRowCount(50);
	_ui.tableWidget_query->setRowCount(50);

	// add a dummy path variable
	_ui.tableWidget_path->setItem(0, 0, new QTableWidgetItem("id"));
	_ui.tableWidget_path->setItem(0, 1, new QTableWidgetItem("55"));

	QJsonObject json =
	{
		{"firstName", "John"},
		{"lastName", "Do"}
	};

	_ui.textEdit_body->setText(QJsonDocument(json).toJson(QJsonDocument::JsonFormat::Indented));

	getContentWidget()->layout()->addWidget(widget);

	connect(_node, &model::Node::ready, this, [=]()
		{
			prepareAndSend();
		});

	setMinSize(QSize(480, 300));
	setSize(10, 10);
}

void logic::PayloadNode::prepareAndSend() const
{
	dynamic_cast<model::PayloadNode*>(getModelNode())->setMessage(composeMessage());
	_node->evaluate();
}

QMap<QString, QVariant> logic::PayloadNode::fillFromTable(QTableWidget* tableWidget) const
{
	QMap<QString, QVariant> outputMap;

	for (int i = 0; i < tableWidget->rowCount(); i++)
	{
		auto keyItem = tableWidget->item(i, 0);
		if (keyItem == nullptr) continue;
		auto key = keyItem->text();
		if (key.isEmpty()) continue;
		auto valueItem = tableWidget->item(i, 1);
		if (valueItem == nullptr) continue;
		auto value = valueItem->text();
		outputMap[key] = value;
	}

	return outputMap;
}