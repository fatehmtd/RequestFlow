#include "EndpointNode.h"
#include <QDebug>
#include <QRegularExpression>
#include <QTextStream>


model::EndpointNode::EndpointNode(model::Graph* graph) : model::Node(graph, "Endpoint")
{
	// default values
	setTimeout(3000);
	setContentType("application/json");
	setHttpMethod(HttpMethod::GET);
	setAuthMethod(AuthorizationMethod::BASIC_AUTH);

	_networkAccessManager = new QNetworkAccessManager(this);
	connect(_networkAccessManager, &QNetworkAccessManager::finished, this, &model::EndpointNode::processResponse);
	connect(&_timer, &QTimer::timeout, this, &EndpointNode::onTimeout);
	_timer.setSingleShot(true);

	QList<unsigned int> acceptedCodes;
	acceptedCodes << 200;
	setAcceptedCodes(acceptedCodes);
	QList<unsigned int> rejectedCodes;
	rejectedCodes << 404 << 401 << 500;
	setRejectedCodes(rejectedCodes);
}

model::EndpointNode::~EndpointNode()
{
	if (_networkReply != nullptr)
	{
		_networkReply->abort();
		delete _networkReply;
	}
}

void model::EndpointNode::createModel()
{
    addInputSlot("Input", Slot::CUSTOM);
    addOutputSlot("Output", Slot::CUSTOM);
}

int model::EndpointNode::HttpMethodFromString(const QString &string)
{
    QStringList methods;
    methods << "get" << "post" << "put" << "del" << "patch";
    int index = methods.lastIndexOf(string.toLower());
    return index;
}

model::InputSlot* model::EndpointNode::getInputSlot() const
{
	return getInputSlotsMap().values()[0];
}

model::OutputSlot* model::EndpointNode::getOutputSlot() const
{
	return getOutputSlotsMap().values()[0];
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
	if (!getAcceptedCodes().isEmpty())
		if (!getAcceptedCodes().contains(status)) return false;
	if (!getRejectedCodes().isEmpty())
		if (getRejectedCodes().contains(status)) return false;
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
		QRegularExpression pattern(QString("{%1}").arg(key));
		workingUrl = workingUrl.replace(pattern, pathVars[key].toString());
	}

	// set query params
	auto queryParams = request.getQueryParams();

	for (const auto& key : queryParams.keys())
	{
		QRegularExpression pattern(QString("{%1}").arg(key));
		workingUrl = workingUrl.replace(pattern, queryParams[key].toString());
	}

	QStringList queryStringList;

	/*

	for (auto& key : queryParams.keys())
	{
		queryStringList.append(QString("%1=%2").arg(key).arg(queryParams[key].toString()));
	}

	if (!queryStringList.isEmpty())
	{
		workingUrl = QString("%1?%2").arg(workingUrl).arg(queryStringList.join("&"));
	}*/

	// extract :vars
	/*
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
	}*/

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
	switch (getAuthMethod())
	{
	case AuthorizationMethod::BASIC_AUTH:
	{
		auto token = QString("%1:%2").arg(getBasicAuthUser()).arg(getBasicAuthPassword()).toLocal8Bit().toBase64();
		request.setRawHeader("Authorization", QString("Basic %1").arg(QString(token)).toLocal8Bit());
	}
	break;
	case AuthorizationMethod::BEARER:
	{
		request.setRawHeader("Authorization", QString("Bearer %1").arg(getBearerToken()).toLocal8Bit());
	}
	break;
	}
	return request;
}

void model::EndpointNode::processResponse(QNetworkReply* reply)
{
	_timer.stop();
	if (reply->isOpen())
	{
		QString data(reply->readAll());

		QString fullResponse;
		QTextStream out(&fullResponse, QIODevice::WriteOnly);

		auto msecs = _elapsedTimer.elapsed();
		auto status = reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute).toInt();

		QString methodStr[] = { "GET", "POST", "PUT", "DEL" };

		out << methodStr[getHttpMethod()] << " " << reply->url().toString() << "\n";
		out << "\n";
		out << "Status : " << status << "\n";
		out << "Elapsed-Time : " << msecs << " ms" << "\n";
		for (auto pair : reply->rawHeaderPairs())
		{
			out << QString(pair.first) << " : " << QString(pair.second) << "\n";
		}
		//out << "Content-Length : " << reply->header(QNetworkRequest::KnownHeaders::ContentLengthHeader).toString() << "\n";
		//out << "Content-Type : " << reply->header(QNetworkRequest::KnownHeaders::ContentTypeHeader).toString() << "\n";
		//out << "Content-Encoding : " << reply->rawHeaderPairs().first(). << "\n";
		//out << "Last-Modified : " << reply->header(QNetworkRequest::KnownHeaders::LastModifiedHeader).toString() << "\n";
		//out << "User-Agent : " << reply->header(QNetworkRequest::KnownHeaders::UserAgentHeader).toString() << "\n";
		out << "Server : " << reply->header(QNetworkRequest::KnownHeaders::ServerHeader).toString() << "\n";
		out << "\n\n";
		out << "Response:\n";
		out << data << "\n";
		/*
		if (_networkReply != nullptr)
		{
			_networkReply->abort();
			delete _networkReply;
		}
		//*/
		setConsoleLog(fullResponse);

		if (validateHttpStatus(status))
		{
			auto response = getInputSlot()->getData();
			response.setBody(data);
			getOutputSlot()->setData(response);
			evaluate();
		}
		else
		{
			fail(QString("Invalid HTTP status %1").arg(status));
		}
	}
	else
	{
		fail(QString("Failed to retreive a response from %1").arg(resolveUrl(getUrl()).toString()));
	}
}

void model::EndpointNode::setExpectedPayload(QString& format)
{
	_expectedPayload = format;
}

QString model::EndpointNode::getExpectedPayload() const
{
	return _expectedPayload;
}

void model::EndpointNode::setAcceptedCodes(const QList<unsigned int>& codes)
{
	_acceptedCodes = codes;
}

QList<unsigned int> model::EndpointNode::getAcceptedCodes() const
{
	return _acceptedCodes;
}

void model::EndpointNode::setRejectedCodes(const QList<unsigned int>& codes)
{
	_rejectedCodes = codes;
}

QList<unsigned int> model::EndpointNode::getRejectedCodes() const
{
	return _rejectedCodes;
}

void model::EndpointNode::setAuthMethod(int method)
{
	_authMethod = method;
}

int model::EndpointNode::getAuthMethod() const
{
	return _authMethod;
}

void model::EndpointNode::setBasicAuthUser(const QString& user)
{
	_baUser = user;
}

void model::EndpointNode::setBasicAuthPassword(const QString& pwd)
{
	_baPwd = pwd;
}

QString model::EndpointNode::getBasicAuthUser() const
{
	return _baUser;
}

QString model::EndpointNode::getBasicAuthPassword() const
{
	return _baPwd;
}

void model::EndpointNode::setBearerToken(const QString& token)
{
	_bearerToken = token;
}

QString model::EndpointNode::getBearerToken() const
{
	return _bearerToken;
}

void model::EndpointNode::setExtraHeaders(const QMap<QString, QString>& headers)
{
	_extraHeaders = headers;
}

QMap<QString, QString> model::EndpointNode::getExtraHeaders() const
{
	return _extraHeaders;
}

QMap<QString, QString>& model::EndpointNode::getExtraHeaders()
{
	return _extraHeaders;
}

QJSValue model::EndpointNode::saveToJSValue(PersistenceHandler* handler) const
{
	auto v = Node::saveToJSValue(handler);
	saveChildren<unsigned int>(v, handler, "acceptedCodes", getAcceptedCodes(), [=](unsigned int v)
		{
			return handler->createJsValue(v);
		});
	saveChildren<unsigned int>(v, handler, "rejectedCodes", getRejectedCodes(), [=](unsigned int v)
		{
			return handler->createJsValue(v);
		});

	saveChildren<QString, QString>(v, handler, "extraHeaders", getExtraHeaders(), [=](const QString& key, const QString& v)
		{
			return handler->createJsValue(v);
		});
	return v;
}

bool model::EndpointNode::loadFromJSValue(const QJSValue& v)
{
	Node::loadFromJSValue(v);
	{
		QList<unsigned int> values;
		loadChildren(v, "acceptedCodes", [&values](const QJSValue& v)
			{
				values << v.toUInt();
			});
		setAcceptedCodes(values);
	}
	{
		QList<unsigned int> values;
		loadChildren(v, "rejectedCodes", [&values](const QJSValue& v)
			{
				values << v.toUInt();
			});
		setRejectedCodes(values);
	}
	return true;
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
		out << "Timeout : " << getTimeout() << "\n";
		out << "Status : " << status << "\n";
		out << "Content-Length : " << _networkReply->header(QNetworkRequest::KnownHeaders::ContentLengthHeader).toString() << "\n";
		out << "Content-Type : " << _networkReply->header(QNetworkRequest::KnownHeaders::ContentTypeHeader).toString() << "\n";
		out << "Last-Modified : " << _networkReply->header(QNetworkRequest::KnownHeaders::LastModifiedHeader).toString() << "\n";
		out << "User-Agent : " << _networkReply->header(QNetworkRequest::KnownHeaders::UserAgentHeader).toString() << "\n";
		out << "Server : " << _networkReply->header(QNetworkRequest::KnownHeaders::ServerHeader).toString() << "\n";
		out << "Elapsed-Time : " << msecs << " ms" << "\n";
		out << "\n\n";
		out << data << "\n";
		/*
		if (_networkReply != nullptr)
		{
			_networkReply->abort();
			delete _networkReply;
		}//*/


		setConsoleLog(fullResponse);

		fail(QString("The operation took longer than expected : %1").arg(getTimeout()));
	}
}

QString compactString(const QString& _body)
{
	auto v = model::Message::JSONParse(_body);
	if (!v.isValid() || v.isNull()) return _body;
	return model::Message::JSONStringify(v);
}

QNetworkReply* model::EndpointNode::sendGet(QNetworkRequest request)
{
	return _networkAccessManager->get(request);
}

QNetworkReply* model::EndpointNode::sendPost(QNetworkRequest request)
{
	auto data = compactString(getInputSlot()->getData().getBody());
	return _networkAccessManager->post(request, data.toUtf8());
}

QNetworkReply* model::EndpointNode::sendDel(QNetworkRequest request)
{
	return _networkAccessManager->deleteResource(request);
}

QNetworkReply* model::EndpointNode::sendPut(QNetworkRequest request)
{
	auto data = compactString(getInputSlot()->getData().getBody());
	return _networkAccessManager->put(request, data.toUtf8());
}

void model::EndpointNode::onErrorOccurred(QNetworkReply::NetworkError error)
{
	qDebug() << error;
}

void model::EndpointNode::onGraphStop()
{
	Node::onGraphStop();
	if (_networkReply != nullptr)
	{
		_networkReply->abort();
		delete _networkReply;
		_networkReply = nullptr;
	}
}

QString model::EndpointNode::getUserAgent() const
{
	return _userAgent;
}

void model::EndpointNode::sendPayload()
{
	_networkAccessManager->setTransferTimeout(0); // 0 = no timeout limit
	_elapsedTimer.start();
	_timer.setInterval(getTimeout());
	_timer.start();

	if (_networkReply != nullptr)
	{
		_networkReply->abort();
		delete _networkReply;
	}

	auto request = prepareRequest();
	switch (getHttpMethod())
	{
	case HttpMethod::GET: // GET
		_networkReply = sendGet(request);
		break;
	case HttpMethod::POST: // POST
		_networkReply = sendPost(request);
		break;
	case HttpMethod::PUT: // PUT
		_networkReply = sendPut(request);
		break;
	case HttpMethod::DEL: // DEL
		_networkReply = sendDel(request);
		break;
	}

	connect(_networkReply, &QNetworkReply::errorOccurred, this, &EndpointNode::onErrorOccurred);
}
