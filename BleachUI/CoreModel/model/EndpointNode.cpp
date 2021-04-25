#include "EndpointNode.h"
#include <QDebug>
#include <QRegularExpression>
#include <QTextStream>

model::EndpointNode::EndpointNode(model::Graph* graph) : model::Node(graph, "Endpoint")
{
	// default values
	setTimeout(3000);
	setContentType("application/json");
	setHttpMethod(0);

	_networkAccessManager = new QNetworkAccessManager(this);
	connect(_networkAccessManager, &QNetworkAccessManager::finished, this, &model::EndpointNode::processResponse);	
	connect(&_timer, &QTimer::timeout, this, &EndpointNode::onTimeout);
	_timer.setSingleShot(true);
}

void model::EndpointNode::createModel()
{
	addInputSlot("Input", Slot::CUSTOM);
	addOutputSlot("Output", Slot::CUSTOM);
}

model::InputSlot* model::EndpointNode::getInputSlot() const
{
	return getInputSlots().values()[0];
}

model::OutputSlot* model::EndpointNode::getOutputSlot() const
{
	return getOutputSlots().values()[0];
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
	case 3: // DEL
		_networkReply = sendDel(request);
		break;
	}
}