#include "ViewerNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Slot.h>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

logic::ViewerNode::ViewerNode(model::Node* modelNode) : view::Node(modelNode, "Viewer")
{
	setupUi();
	setTitle("Viewer");
}

QJSValue logic::ViewerNode::toJSValue(QJSEngine& engine) const
{
	auto value = Node::toJSValue(engine);
	value.setProperty("_currentTab", _ui.tabWidget->currentIndex());
	return value;
}

void logic::ViewerNode::clearUI()
{
	_ui.textEdit_raw->clear();
	_ui.textEdit_json->clear();
}

void logic::ViewerNode::setupUi()
{
	auto widget = new QWidget();
	_ui.setupUi(widget);

	getContentWidget()->layout()->addWidget(widget);
	_bgColor = view::colors::green;
	connect(_node, &model::Node::ready, this, [=]()
		{
			auto request = _node->getInputSlots().first()->getData();

			auto data = request.getBody();
			
			QJsonDocument document = QJsonDocument::fromJson(data.toUtf8());

			_ui.textEdit_raw->setPlainText(data);
			_ui.textEdit_json->setPlainText(document.toJson());
			
			_node->evaluate();
		});

	setMinSize(QSize(400, 300));
	setSize(300, 200);	
}
