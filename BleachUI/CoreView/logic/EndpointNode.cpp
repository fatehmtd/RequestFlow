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
	_ui.plainTextEdit_json->clear();
	_ui.plainTextEdit_raw->clear();
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

			auto data = node->getInputSlot()->getData().getBody();
			_ui.plainTextEdit_raw->setPlainText(data);
			_ui.plainTextEdit_json->setPlainText(data);
			node->sendPayload();
		});

	setMinSize(QSize(280, 200));
	setSize(300, 100);
}

model::EndpointNode::EndpointNode(model::Graph* graph) : model::Node(graph, "Endpoint")
{
	_networkAccessManager = new QNetworkAccessManager(this);
	connect(_networkAccessManager, &QNetworkAccessManager::finished, this, &model::EndpointNode::processResponse);	
	connect(&_timer, &QTimer::timeout, this, &EndpointNode::onTimeout);
	_timer.setSingleShot(true);
}

void model::EndpointNode::createModel()
{
	_inputSlot = addInputSlot("Input", Slot::CUSTOM);
	_outputSlot = addOutputSlot("Output", Slot::CUSTOM);
}

model::InputSlot* model::EndpointNode::getInputSlot() const
{
	return _inputSlot;
}

model::OutputSlot* model::EndpointNode::getOutputSlot() const
{
	return _outputSlot;
}

void model::EndpointNode::setTimeout(int msec)
{
	_timeout = msec;
	emit timeoutChanged(msec);
}

int model::EndpointNode::getTimeout() const
{
	return _timeout;
}

void model::EndpointNode::setUrl(const QString& url)
{
	_url = url;
	emit urlChanged(url);
}

QString model::EndpointNode::getUrl() const
{
	return _url;
}

void model::EndpointNode::setContentType(const QString& contentType)
{
	_contentType = contentType;
	emit contentTypeChanged(contentType);
}

QString model::EndpointNode::getContentType() const
{
	return _contentType;
}

void model::EndpointNode::setConsoleLog(const QString& log)
{
	_consoleLog = log;
	emit consoleLogChanged(log);
}

QString model::EndpointNode::getConsoleLog() const
{
	return _consoleLog;
}

void model::EndpointNode::setHttpMethod(int method)
{
	_httpMethod = method;
	emit httpMethodChanged(method);
}

int model::EndpointNode::getHttpMethod() const
{
	return _httpMethod;
}

void model::EndpointNode::setUserAgent(const QString& userAgent)
{
	_userAgent = userAgent;
	emit userAgentChanged(userAgent);
}

bool model::EndpointNode::validateHttpStatus(int status) const
{
	return true;
}

QUrl model::EndpointNode::resolveUrl(const QString& rawUrl) const
{
	auto request = getInputSlot()->getData();
	//request.printMe();

	QString workingUrl = rawUrl;

	// replace path variables placeholders
	const auto& pathVars = request.getPathVars();
	for (const auto& key : pathVars.keys())
	{
		QRegularExpression pattern(QString(":(%1)").arg(key));
		workingUrl = workingUrl.replace(pattern, pathVars[key].toString());
	}

	// set query params
	auto queryParams = request.getQueryParams();
	QStringList queryStringList;

	for (auto& key : queryParams.keys())
	{
		queryStringList.append(QString("%1=%2").arg(key).arg(queryParams[key].toString()));
	}

	if (!queryStringList.isEmpty())
	{
		workingUrl = QString("%1?%2").arg(workingUrl).arg(queryStringList.join("&"));
	}

	// extract :vars
	QList<QString> pathVariablesPlaceHolders;
	{
		QRegularExpression pathVarPattern(":([\\d\\w]+)");
		auto it = pathVarPattern.globalMatch(rawUrl);
		while (it.hasNext())
		{
			auto match = it.next();
			auto name = match.captured(1);
			pathVariablesPlaceHolders.push_back(match.captured());
		}
	}

	auto environment = getGraph()->getActiveEnvironment();

	if (environment != nullptr)
	{
		workingUrl = environment->evaluate(workingUrl);
	}

	return QUrl(workingUrl);
}

QNetworkRequest model::EndpointNode::prepareRequest()
{
	QNetworkRequest request(resolveUrl(getUrl()));
	request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, getContentType());
	request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, getUserAgent());
	return request;
}

void model::EndpointNode::processResponse(QNetworkReply* reply)
{
	_timer.stop();

	QString data(reply->readAll());

	QString fullResponse;
	QTextStream out(&fullResponse, QIODevice::WriteOnly);

	auto msecs = _elapsedTimer.elapsed();
	auto status = reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute).toInt();

	QString methodStr[] = {"GET", "POST", "PUT", "PATCH", "DEL"};

	out << methodStr[getHttpMethod()] << " " << reply->url().toString() << "\n";
	out << "\n";
	out << "Status : " << status << "\n";
	out << "Content-Length : " << reply->header(QNetworkRequest::KnownHeaders::ContentLengthHeader).toString() << "\n";
	out << "Content-Type : " << reply->header(QNetworkRequest::KnownHeaders::ContentTypeHeader).toString() << "\n";
	out << "Last-Modified : " << reply->header(QNetworkRequest::KnownHeaders::LastModifiedHeader).toString() << "\n";
	out << "User-Agent : " << reply->header(QNetworkRequest::KnownHeaders::UserAgentHeader).toString() << "\n";
	out << "Server : " << reply->header(QNetworkRequest::KnownHeaders::ServerHeader).toString() << "\n";
	out << "Elapsed-Time : " << msecs << " ms" << "\n";
	out << "\n\n";
	out << data << "\n";

	setConsoleLog(fullResponse);

	if (validateHttpStatus(status))
	{
		getOutputSlot()->setData(model::Message(data));
		evaluate();
	}
	else
	{
		raiseException(QString("Invalid HTTP status %1").arg(status));
	}
}

void model::EndpointNode::onTimeout()
{
	if (_networkReply != nullptr)
	{
		_networkReply->abort();
		QString data(_networkReply->readAll());

		QString fullResponse;
		QTextStream out(&fullResponse, QIODevice::WriteOnly);

		auto msecs = _elapsedTimer.elapsed();
		auto status = _networkReply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute).toInt();

 		QString methodStr[] = { "GET", "POST", "PUT", "PATCH", "DEL" };

		out << methodStr[getHttpMethod()] << " " << _networkReply->url().toString() << "\n";
		out << "\n";
		out << "Status : " << status << "\n";
		out << "Content-Length : " << _networkReply->header(QNetworkRequest::KnownHeaders::ContentLengthHeader).toString() << "\n";
		out << "Content-Type : " << _networkReply->header(QNetworkRequest::KnownHeaders::ContentTypeHeader).toString() << "\n";
		out << "Last-Modified : " << _networkReply->header(QNetworkRequest::KnownHeaders::LastModifiedHeader).toString() << "\n";
		out << "User-Agent : " << _networkReply->header(QNetworkRequest::KnownHeaders::UserAgentHeader).toString() << "\n";
		out << "Server : " << _networkReply->header(QNetworkRequest::KnownHeaders::ServerHeader).toString() << "\n";
		out << "Elapsed-Time : " << msecs << " ms" << "\n";
		out << "\n\n";
		out << data << "\n";

		setConsoleLog(fullResponse);
	}
}

QNetworkReply* model::EndpointNode::sendGet(QNetworkRequest request)
{
	return _networkAccessManager->get(request);
}

QNetworkReply* model::EndpointNode::sendPost(QNetworkRequest request)
{
	auto data = getInputSlot()->getData().getBody();
	return _networkAccessManager->post(request, data.toUtf8());
}

QNetworkReply* model::EndpointNode::sendDel(QNetworkRequest request)
{
	return _networkAccessManager->deleteResource(request);
}

QNetworkReply* model::EndpointNode::sendPut(QNetworkRequest request)
{
	auto data = getInputSlot()->getData().getBody();
	return _networkAccessManager->put(request, data.toUtf8());
}

QNetworkReply* model::EndpointNode::sendPatch(QNetworkRequest request)
{
	auto data = getInputSlot()->getData().getBody();
	return _networkAccessManager->sendCustomRequest(request, "PATCH", data.toUtf8());
}

QString model::EndpointNode::getUserAgent() const
{
	return _userAgent;
}

void model::EndpointNode::sendPayload()
{
	_elapsedTimer.start();
	_timer.setInterval(getTimeout());
	_timer.start();

	auto request = prepareRequest();
	switch (getHttpMethod())
	{
	case 0: // GET
		_networkReply = sendGet(request);
		break;
	case 1: // POST
		_networkReply = sendPost(request);
		break;
	case 2: // PUT
		_networkReply = sendPut(request);
		break;
	case 3: // PATCH
		_networkReply = sendPatch(request);
		break;
	case 4: // DEL
		_networkReply = sendDel(request);
		break;
	}
}