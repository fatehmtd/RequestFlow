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

	auto endpointNode = dynamic_cast<model::EndpointNode*>(getModelNode());
	//_ui.lineEdit_url->setText(QString("https://jsonplaceholder.typicode.com/posts"));
	_ui.lineEdit_url->setText(endpointNode->getUrl());
	for (int i = 0; i < _ui.comboBox_contentType->count(); i++)
	{
		if (endpointNode->getContentType() == _ui.comboBox_contentType->itemText(i))
		{
			_ui.comboBox_contentType->setCurrentIndex(i);
			break;
		}
	}
	_ui.comboBox_method->setCurrentIndex(endpointNode->getHttpMethod());
	_ui.spinBox_timeout->setValue(endpointNode->getTimeout());	
	//_ui.spinBox_timeout->setValue(endpointNode->getTimeout()); // user agent
	_ui.comboBox_method->setCurrentIndex(0);


	// connect signals
	connect(_ui.lineEdit_url, &QLineEdit::textChanged, endpointNode, &model::EndpointNode::setUrl);
	connect(_ui.spinBox_timeout, SIGNAL(valueChanged(int)), this, SLOT(onTimeoutChanged(int)));
	connect(_ui.comboBox_method, SIGNAL(currentIndexChanged(int)), this, SLOT(onHttpMethodChanged(int)));
	connect(_ui.comboBox_contentType, SIGNAL(currentTextChanged(const QString&)), this, SLOT(onContentTypeChanged(const QString&)));

	connect(node, &model::EndpointNode::consoleLogChanged, _ui.plainTextEdit_response, &QPlainTextEdit::setPlainText);

	_ui.plainTextEdit_response->setPalette(QPalette(Qt::black));
	_ui.plainTextEdit_response->setBackgroundVisible(true);

	_bgColor = view::colors::blue;
	connect(_node, &model::Node::ready, this, [=]()
		{
			auto node = dynamic_cast<model::EndpointNode*>(getModelNode());
			node->setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36 OPR/74.0.3911.107");
			node->sendPayload();
		});

	setMinSize(QSize(280, 200));
	setSize(300, 100);
}

void logic::EndpointNode::onTimeoutChanged(int delay)
{
	getModelNode<model::EndpointNode*>()->setTimeout(delay);
}

void logic::EndpointNode::onContentTypeChanged(const QString& t)
{
	getModelNode<model::EndpointNode*>()->setContentType(t);
}

void logic::EndpointNode::onHttpMethodChanged(int index)
{
	getModelNode<model::EndpointNode*>()->setHttpMethod(index);
}