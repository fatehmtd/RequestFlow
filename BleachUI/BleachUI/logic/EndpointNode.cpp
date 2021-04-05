#include "EndpointNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Environment.h>
#include <QDebug>
#include <QRegularExpression>


logic::EndpointNode::EndpointNode(model::Node* modelNode) : view::Node(modelNode, "Endpoint"), _ui(new Ui::EndpointNodeUiWidget)
{
	_networkAccessManager = new QNetworkAccessManager(this);
	connect(_networkAccessManager, &QNetworkAccessManager::finished, this, &EndpointNode::replyReceived);
	initUI();
	setTitle("Endpoint");
}

QJSValue logic::EndpointNode::toJSValue(QJSEngine& engine) const
{
	auto value = Node::toJSValue(engine);
	value.setProperty("_url", _ui->lineEdit_url->text());
	value.setProperty("_method", _ui->comboBox_method->currentIndex());
	value.setProperty("_contentTyoe", _ui->comboBox_contentType->currentIndex());
	return value;
}

void logic::EndpointNode::fromJSValue(const QJSValue& jsValue)
{
	Node::fromJSValue(jsValue);
	_ui->lineEdit_url->setText(jsValue.property("_url").toString());
	_ui->comboBox_method->setCurrentIndex(jsValue.property("_method").toInt());
	_ui->comboBox_contentType->setCurrentIndex(jsValue.property("_contentTyoe").toInt());
}

void logic::EndpointNode::setTimeout(unsigned int sec)
{
}

void logic::EndpointNode::setUrl(const QUrl& url)
{
}

void logic::EndpointNode::setMethod(HttpMethod method)
{
}

void logic::EndpointNode::clearUI()
{
	_ui->plainTextEdit_response->clear();
	_ui->plainTextEdit_json->clear();
	_ui->plainTextEdit_raw->clear();
}

void logic::EndpointNode::initUI()
{
	auto widget = new QWidget();
	_ui->setupUi(widget);
	getContentWidget()->layout()->addWidget(widget);
	getContentWidget()->adjustSize();

	//_ui->lineEdit_url->setText("http://localhost:8080/testPost");
	//_ui->lineEdit_url->setText(QString("{{baseUrl}}/testPost/:id"));
	_ui->lineEdit_url->setText(QString("https://jsonplaceholder.typicode.com/posts"));
	_ui->comboBox_method->setCurrentIndex(0);
	//_ui->comboBox_method->setCurrentIndex(1);

	auto inputSlots = _node->getInputSlots();
	auto outputSlots = _node->getOutputSlots();

	_input = inputSlots.first();
	_output = outputSlots.first();

	_bgColor = view::colors::blue;
	connect(_node, &model::Node::ready, this, [=]()
		{
			sendPayload();
		});

	setMinSize(QSize(280, 200));
	setSize(300, 100);
}

void logic::EndpointNode::sendPayload()
{
	auto body = _input->getData().getBody();
	_ui->plainTextEdit_json->setPlainText(body);
	_ui->plainTextEdit_raw->setPlainText(body);

	_elapsedTimer.start();
	switch (_ui->comboBox_method->currentIndex())
	{
	case 0: // GET
		sendGet();
		break;
	case 1: // POST
		sendPost();
		break;
	case 2: // PUT
		sendPut();
		break;
	case 3: // PATCH
		sendPatch();
		break;
	case 4: // DEL
		sendDel();
		break;
	}
}

void logic::EndpointNode::sendGet()
{
	QNetworkRequest request(resolveUrl(_ui->lineEdit_url->text()));
	request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, _ui->comboBox_contentType->currentText());
	request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36 OPR/74.0.3911.107");

	auto reply = _networkAccessManager->get(request);
}

void logic::EndpointNode::sendPost()
{
	auto data = _input->getData().getBody();

	QNetworkRequest request(resolveUrl(_ui->lineEdit_url->text()));
	request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, _ui->comboBox_contentType->currentText());
	request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36 OPR/74.0.3911.107");

	//QUrlQuery query(data);
	//auto reply = _networkAccessManager->post(request, query.toString(QUrl::ComponentFormattingOption::FullyEncoded).toLatin1());
	auto reply = _networkAccessManager->post(request, data.toUtf8());
}

void logic::EndpointNode::sendDel()
{
	auto data = _input->getData().getBody();

	QNetworkRequest request(resolveUrl(_ui->lineEdit_url->text()));
	request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, _ui->comboBox_contentType->currentText());
	request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36 OPR/74.0.3911.107");

	//QUrlQuery query(data);
	auto reply = _networkAccessManager->deleteResource(request);
}

#include <QTextStream>

void logic::EndpointNode::sendPut()
{
	auto data = _input->getData().getBody();

	QNetworkRequest request(resolveUrl(_ui->lineEdit_url->text()));
	request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, _ui->comboBox_contentType->currentText());
	request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36 OPR/74.0.3911.107");

	//QUrlQuery query(data);
	//auto reply = _networkAccessManager->post(request, query.toString(QUrl::ComponentFormattingOption::FullyEncoded).toLatin1());
	auto reply = _networkAccessManager->put(request, data.toUtf8());
}

void logic::EndpointNode::sendPatch()
{
	auto data = _input->getData().getBody();

	QNetworkRequest request(resolveUrl(_ui->lineEdit_url->text()));
	request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, _ui->comboBox_contentType->currentText());
	request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36 OPR/74.0.3911.107");

	//QUrlQuery query(data);

	auto reply = _networkAccessManager->sendCustomRequest(request, "PATCH", data.toUtf8());
}

bool logic::EndpointNode::validateHttpStatus(int status) const
{
	return status == 200;
}

QUrl logic::EndpointNode::resolveUrl(const QString& rawUrl) const
{
	auto request = _input->getData();
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

	auto environment = getSceneGraph()->getModelGraph()->getActiveEnvironment();

	if (environment != nullptr)
	{
		workingUrl = environment->evaluate(workingUrl);
	}

	return QUrl(workingUrl);
}

void logic::EndpointNode::processResponse(QNetworkReply* reply)
{
	QString data(reply->readAll());

	QString fullResponse;
	QTextStream out(&fullResponse, QIODevice::WriteOnly);

	auto msecs = _elapsedTimer.elapsed();
	auto status = reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute).toInt();

	out << _ui->comboBox_method->currentText() << " " << reply->url().toString() << "\n";
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

	_ui->plainTextEdit_response->setPlainText(fullResponse);

	if (validateHttpStatus(status))
	{
		_output->setData(model::Message(data));
		_node->evaluate();
	}
	else
	{
		_node->raiseException(QString("Invalid http status %1").arg(status));
	}
}

void logic::EndpointNode::replyReceived(QNetworkReply* reply)
{
	processResponse(reply);
}
