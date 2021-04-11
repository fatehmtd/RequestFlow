#include "EndpointNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Environment.h>
#include <QDebug>
#include <QRegularExpression>
#include <QTextStream>

logic::EndpointNode::EndpointNode(model::EndpointNode* modelNode) : view::Node(modelNode, "Endpoint")
{
	initUI();
	setTitle("Endpoint");
}

QJSValue logic::EndpointNode::toJSValue(QJSEngine& engine) const
{
	auto value = Node::toJSValue(engine);
	value.setProperty("_url", _ui.lineEdit_url->text());
	value.setProperty("_method", _ui.comboBox_method->currentIndex());
	value.setProperty("_contentTyoe", _ui.comboBox_contentType->currentIndex());
	return value;
}

void logic::EndpointNode::fromJSValue(const QJSValue& jsValue)
{
	Node::fromJSValue(jsValue);
	_ui.lineEdit_url->setText(jsValue.property("_url").toString());
	_ui.comboBox_method->setCurrentIndex(jsValue.property("_method").toInt());
	_ui.comboBox_contentType->setCurrentIndex(jsValue.property("_contentTyoe").toInt());
}

void logic::EndpointNode::clearUI()
{
	_ui.plainTextEdit_response->clear();
}

void logic::EndpointNode::initUI()
{	
	auto widget = new QWidget();
	_ui.setupUi(widget);
	getContentWidget()->layout()->addWidget(widget);
	getContentWidget()->adjustSize();
	 
	auto node = dynamic_cast<model::EndpointNode*>(getModelNode());	

	_ui.lineEdit_url->setText(QString("https://jsonplaceholder.typicode.com/posts"));
	_ui.comboBox_method->setCurrentIndex(0);

	connect(node, &model::EndpointNode::consoleLogChanged, _ui.plainTextEdit_response, &QPlainTextEdit::setPlainText);

	_ui.plainTextEdit_response->setPalette(QPalette(Qt::black));
	_ui.plainTextEdit_response->setBackgroundVisible(true);

	_bgColor = view::colors::blue;
	connect(_node, &model::Node::ready, this, [=]()
		{
			auto node = dynamic_cast<model::EndpointNode*>(getModelNode());
			node->setContentType(_ui.comboBox_contentType->currentText());
			node->setHttpMethod(_ui.comboBox_method->currentIndex());
			node->setUrl(_ui.lineEdit_url->text());
			node->setTimeout(_ui.spinBox_timeout->value());

			node->setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36 OPR/74.0.3911.107");
			node->sendPayload();
		});

	setMinSize(QSize(280, 200));
	setSize(300, 100);
}