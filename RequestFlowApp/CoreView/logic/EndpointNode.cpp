#include "EndpointNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Environment.h>
#include <QDebug>
#include <QRegularExpression>
#include <QTextStream>

#include <QJsonDocument>

logic::EndpointNode::EndpointNode(model::EndpointNode* modelNode) : view::Node(modelNode, "Endpoint")
{
	initUI();
}

void logic::EndpointNode::clearUI()
{
    //_ui.plainTextEdit_request->clear();
	_ui.plainTextEdit_response->clear();
}

void logic::EndpointNode::initUI()
{
	auto widget = new QWidget();
	_ui.setupUi(widget);
	getContentWidget()->layout()->addWidget(widget);
	getContentWidget()->adjustSize();

	connect(_ui.lineEdit_url, &QLineEdit::textChanged, this, &EndpointNode::onUrlTextChanged);

	// set the data from the model
	auto endpointNode = dynamic_cast<model::EndpointNode*>(getModelNode());
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
	_ui.plainTextEdit_expectedPayload->setPlainText(endpointNode->getExpectedPayload());
	_ui.tabWidget_authentication->setCurrentIndex(endpointNode->getAuthMethod());
	_ui.lineEdit_baUser->setText(endpointNode->getBasicAuthUser());
	_ui.lineEdit_baPwd->setText(endpointNode->getBasicAuthPassword());
    _ui.lineEdit_bearerToken->setText(endpointNode->getBearerToken());

    // Http codes
	{
		_ui.tableWidget_acceptedCodes->clearContents();
		auto codes = endpointNode->getAcceptedCodes();
		for (int i = 0; i < codes.size(); i++)
		{
			_ui.tableWidget_acceptedCodes->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(codes[i])));
		}
	}
	{
		_ui.tableWidget_rejectedCodes->clearContents();
		auto codes = endpointNode->getRejectedCodes();
		for (int i = 0; i < codes.size(); i++)
		{
			_ui.tableWidget_rejectedCodes->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(codes[i])));
		}
	}

    // Extra headers
    {
        _ui.tableWidget_extraHeaders->clearContents();
        auto extraHeaders = endpointNode->getExtraHeaders();
        int index = 0;
        for(auto key : extraHeaders.keys())
        {
            auto value = extraHeaders[key];
            if(value.isEmpty()) continue;
            _ui.tableWidget_extraHeaders->setItem(index, 0, new QTableWidgetItem(key));
            _ui.tableWidget_extraHeaders->setItem(index, 1, new QTableWidgetItem(value));
            index++;
        }
    }

	// connect signals
	connect(_ui.lineEdit_url, &QLineEdit::textChanged, endpointNode, &model::EndpointNode::setUrl);
	connect(_ui.lineEdit_baUser, &QLineEdit::textChanged, endpointNode, &model::EndpointNode::setBasicAuthUser);
	connect(_ui.lineEdit_baPwd, &QLineEdit::textChanged, endpointNode, &model::EndpointNode::setBasicAuthPassword);
	connect(_ui.lineEdit_bearerToken, &QLineEdit::textChanged, endpointNode, &model::EndpointNode::setBearerToken);
	connect(_ui.tabWidget_authentication, &QTabWidget::currentChanged, endpointNode, &model::EndpointNode::setAuthMethod);
	connect(_ui.spinBox_timeout, SIGNAL(valueChanged(int)), this, SLOT(onTimeoutChanged(int)));
	connect(_ui.comboBox_method, SIGNAL(currentIndexChanged(int)), this, SLOT(onHttpMethodChanged(int)));
	connect(_ui.comboBox_contentType, SIGNAL(currentTextChanged(const QString&)), this, SLOT(onContentTypeChanged(const QString&)));

    connect(_ui.tableWidget_extraHeaders, &QTableWidget::itemChanged, this, [=](QTableWidgetItem* item)
        {/*
                QString value = item->data(Qt::DisplayRole).toString();

                // validate the value
                // check if already existing

                if (endpointNode->getExtraHeaders().contains(value))
                {
                    _ui.tableWidget_extraHeaders->blockSignals(true);
                    item->setData(Qt::DisplayRole, ""); // clear the cell
                    _ui.tableWidget_extraHeaders->blockSignals(false);
                }*/
                //else // set the values in the model
                {
                    QMap<QString, QString> values;
                    for (auto i = 0; i < _ui.tableWidget_extraHeaders->rowCount(); i++)
                    {
                        auto itemKey = _ui.tableWidget_extraHeaders->item(i, 0);
                        if (itemKey == nullptr) continue;
                        auto itemValue = _ui.tableWidget_extraHeaders->item(i, 1);
                        if (itemValue == nullptr) continue;
                        auto key = itemKey->data(Qt::DisplayRole).toString();
                        if(key.isEmpty()) continue;
                        auto value = itemValue->data(Qt::DisplayRole).toString();
                        values[key] = value;
                    }
                    endpointNode->setExtraHeaders(values);
                }
            });

	connect(_ui.tableWidget_acceptedCodes, &QTableWidget::itemChanged, this, [=](QTableWidgetItem* item) 
		{
			int value = item->data(Qt::DisplayRole).toInt();

			// validate the value
			// check if already existing

			if (value < 100 || endpointNode->getAcceptedCodes().contains(value) || endpointNode->getRejectedCodes().contains(value))
			{
				_ui.tableWidget_acceptedCodes->blockSignals(true);
				item->setData(Qt::DisplayRole, ""); // clear the cell
				_ui.tableWidget_acceptedCodes->blockSignals(false);
			}
			//else // set the values in the model
			{
				QList<unsigned int> values;
				for (auto i = 0; i < _ui.tableWidget_acceptedCodes->rowCount(); i++)
				{
					auto item = _ui.tableWidget_acceptedCodes->item(i, 0);
					if (item == nullptr) continue;
					auto v = item->data(Qt::DisplayRole).toInt();
					if (v < 100) continue;
					values << v;
				}
				endpointNode->setAcceptedCodes(values);
			}
		});

	connect(_ui.tableWidget_rejectedCodes, &QTableWidget::itemChanged, this, [=](QTableWidgetItem* item) 
		{
			int value = item->data(Qt::DisplayRole).toInt();

			// validate the value
            // check if already existing
			if (value < 100 || endpointNode->getAcceptedCodes().contains(value) || endpointNode->getRejectedCodes().contains(value))
			{
				_ui.tableWidget_rejectedCodes->blockSignals(true);
				item->setData(Qt::DisplayRole, ""); // clear the cell
				_ui.tableWidget_rejectedCodes->blockSignals(false);
			}
			//else // set the values in the model
			{
				QList<unsigned int> values;
				for (auto i = 0; i < _ui.tableWidget_rejectedCodes->rowCount(); i++)
				{
					auto item = _ui.tableWidget_rejectedCodes->item(i, 0);
					if (item == nullptr) continue;
					auto v = item->data(Qt::DisplayRole).toInt();
					if (v < 100) continue;
					values << v;
				}
				endpointNode->setRejectedCodes(values);
			}
		});

    connect(endpointNode, &model::EndpointNode::consoleResponseLogChanged, _ui.plainTextEdit_response, &QPlainTextEdit::setPlainText);
    connect(endpointNode, &model::EndpointNode::consoleRequestLogChanged, _ui.plainTextEdit_request, &QPlainTextEdit::setPlainText);

	//_ui.plainTextEdit_response->setPalette(QPalette(Qt::black));
	//_ui.plainTextEdit_response->setStyleSheet("color: white; background-color: black;");
	//_ui.plainTextEdit_response->setBackgroundVisible(true);

    //_bgColor = view::colors::blue;
    _bgColor = view::colors::nodes::endpoint;
    setSvgIcon((":/nodes/endpoint"));
	connect(_node, &model::Node::ready, this, [=]()
		{
			auto node = dynamic_cast<model::EndpointNode*>(getModelNode());

			node->info(QString("[Preparing] %1:%2").arg(node->getType()).arg(node->getName()));

            //_ui.plainTextEdit_request->setPlainText(node->getInputSlot()->getData().getBody());

			//TODO make the user agent dynamic
			node->setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36 OPR/74.0.3911.107");
			node->sendPayload();
		});

    setMinSize(QSize(500, 375));
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

#include <QRegularExpression>
#include <QDebug>

#include <QTextCursor>

void logic::EndpointNode::onUrlTextChanged(const QString& rawUrl)
{
    QString workingUrl = rawUrl;
/*
	QRegularExpression pattern("({[\\w\\d]+})");

	auto globalMatch = pattern.globalMatch(rawUrl);    

	while(globalMatch.hasNext())
	{
		auto match = globalMatch.next();

		for (auto m : match.capturedTexts())
		{
			//auto m = match.captured(0);
			int l = m.size();
			int index = rawUrl.indexOf(m);
			//_ui.lineEdit_url->settext
        }
	}

	QStringList queryStringList;
*/
	auto environment = getModelNode()->getGraph()->getActiveEnvironment();

	if (environment != nullptr)
	{
		workingUrl = environment->evaluate(workingUrl);
	}
}
