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

model::Message logic::PayloadNode::composeMessage() const
{
	model::Message message;
	message.setBody(_ui.plainTextEdit_body->toPlainText());
	message.setPathVars(fillFromTable(_ui.tableWidget_path));
	message.setQueryParams(fillFromTable(_ui.tableWidget_query));

	//message.printMe();
	return message;
}

void logic::PayloadNode::fillFromMessage(const model::Message& message)
{	
	_ui.tableWidget_path->setRowCount(0);
	_ui.tableWidget_query->setRowCount(0);

	_ui.tableWidget_path->setRowCount(50);
	_ui.tableWidget_query->setRowCount(50);

	{
		auto keys = message.getQueryParams().keys();
		for (int i = 0; i < keys.size(); i++)
		{
			_ui.tableWidget_query->setItem(i, 0, new QTableWidgetItem(keys[i]));
			_ui.tableWidget_query->setItem(i, 1, new QTableWidgetItem((message.getQueryParams()[keys[i]]).toString()));
		}
	}

	{
		auto keys = message.getPathVars().keys();
		for (int i = 0; i < keys.size(); i++)
		{
			_ui.tableWidget_path->setItem(i, 0, new QTableWidgetItem(keys[i]));
			_ui.tableWidget_path->setItem(i, 1, new QTableWidgetItem((message.getPathVars()[keys[i]]).toString()));
		}
	}

	_ui.plainTextEdit_body->setPlainText(message.getBody());
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

	auto message = dynamic_cast<model::PayloadNode*>(getModelNode())->getMessage();

	fillFromMessage(message);

	getContentWidget()->layout()->addWidget(widget);

	connect(_node, &model::Node::ready, this, &PayloadNode::prepareAndSend);

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