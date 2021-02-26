#include "EndpointNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <QDebug>

logic::EndpointNode::EndpointNode(model::Node* modelNode) : view::Node(modelNode), _ui(new Ui::EndpointNodeUiWidget)
{
	_networkAccessManager = new QNetworkAccessManager(this);
	connect(_networkAccessManager, &QNetworkAccessManager::finished, this, &EndpointNode::replyReceived);
	setupUi();
	setTitle("Endpoint");
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

void logic::EndpointNode::setupUi()
{
	auto widget = new QWidget();
	_ui->setupUi(widget);
	getContentWidget()->layout()->addWidget(widget);

	_ui->lineEdit_url->setText("http://localhost:8080/testPost");
	_ui->comboBox_method->setCurrentIndex(1);

	_input = _node->getInputSlots().first();
	_output = _node->getOutputSlots().first();

	_bgColor = view::colors::blue;
	connect(_node, &model::Node::ready, this, [=]() {
		sendPayload();
		});

	setSize(300, 400);
}

void logic::EndpointNode::sendPayload()
{
	_ui->plainTextEdit_json->setPlainText(_input->getData().toString());
	_ui->plainTextEdit_raw->setPlainText(_input->getData().toString());

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
	QNetworkRequest request(QUrl(_ui->lineEdit_url->text()));
	request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, _ui->comboBox_contentType->currentText());
	request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36 OPR/74.0.3911.107");

	auto reply = _networkAccessManager->get(request);
}

void logic::EndpointNode::sendPost()
{
	auto data = _input->getData().toString();
	
	QNetworkRequest request(QUrl(_ui->lineEdit_url->text()));
	request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, _ui->comboBox_contentType->currentText());
	request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36 OPR/74.0.3911.107");
	
	QUrlQuery query(_input->getData().toString());
	//auto reply = _networkAccessManager->post(request, query.toString(QUrl::ComponentFormattingOption::FullyEncoded).toLatin1());
	auto reply = _networkAccessManager->post(request, data.toUtf8());
}

void logic::EndpointNode::sendDel()
{
	auto data = _input->getData().toString();

	QNetworkRequest request(QUrl(_ui->lineEdit_url->text()));
	request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, _ui->comboBox_contentType->currentText());
	request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36 OPR/74.0.3911.107");

	QUrlQuery query(_input->getData().toString());
	auto reply = _networkAccessManager->deleteResource(request);

}

#include <QTextStream>

void logic::EndpointNode::sendPut()
{
	auto data = _input->getData().toString();

	QNetworkRequest request(QUrl(_ui->lineEdit_url->text()));
	request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, _ui->comboBox_contentType->currentText());
	request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36 OPR/74.0.3911.107");

	QUrlQuery query(_input->getData().toString());
	//auto reply = _networkAccessManager->post(request, query.toString(QUrl::ComponentFormattingOption::FullyEncoded).toLatin1());
	auto reply = _networkAccessManager->put(request, data.toUtf8());
}

void logic::EndpointNode::sendPatch()
{
	auto data = _input->getData().toString();

	QNetworkRequest request(QUrl(_ui->lineEdit_url->text()));
	request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, _ui->comboBox_contentType->currentText());
	request.setHeader(QNetworkRequest::KnownHeaders::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.150 Safari/537.36 OPR/74.0.3911.107");

	QUrlQuery query(_input->getData().toString());

	auto reply = _networkAccessManager->sendCustomRequest(request, "PATCH", data.toUtf8());
}

bool logic::EndpointNode::validateHttpStatus(int status) const
{
	return status == 200;
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

	_output->setData(data);

	if (validateHttpStatus(status))
		_node->evaluate();
	else
		_node->raiseException(QString("Invalid http status %1").arg(status));
}

void logic::EndpointNode::replyReceived(QNetworkReply* reply)
{
	processResponse(reply);

	//_node->evaluate();
}
