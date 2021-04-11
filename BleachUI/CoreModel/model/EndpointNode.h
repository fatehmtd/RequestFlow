#pragma once
#include "../coremodel_global.h"
#include "Node.h"
#include "Message.h"
#include "Graph.h"
#include "Slot.h"
#include "Environment.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QTimer>
#include <QElapsedTimer>

namespace model
{
    class COREMODEL_EXPORT EndpointNode : public model::Node
    {
        Q_OBJECT
        Q_PROPERTY(QString url MEMBER _url READ getUrl WRITE setUrl NOTIFY urlChanged)
        Q_PROPERTY(int timeout MEMBER _timeout READ getTimeout WRITE setTimeout NOTIFY timeoutChanged)
        Q_PROPERTY(int httpMethod MEMBER _httpMethod READ getHttpMethod WRITE setHttpMethod NOTIFY httpMethodChanged)
        Q_PROPERTY(QString contentType MEMBER _contentType READ getContentType WRITE setContentType NOTIFY contentTypeChanged)
        Q_PROPERTY(QString consoleLog MEMBER _consoleLog READ getConsoleLog WRITE setConsoleLog NOTIFY consoleLogChanged)
        Q_PROPERTY(QString userAgent MEMBER _userAgent READ getUserAgent WRITE setUserAgent NOTIFY userAgentChanged)
    public:
        EndpointNode(model::Graph* graph);
        
        void createModel() override;

		enum HttpMethod
		{
			GET,
			POST,
			PUT,
			PATCH,
			DEL
		};
        Q_ENUM(HttpMethod);

		InputSlot* getInputSlot() const;
		OutputSlot* getOutputSlot() const;

		void setTimeout(int msec);
		int getTimeout() const;

		void setUrl(const QString& url);
		QString getUrl() const;

		void setContentType(const QString& contentType);
		QString getContentType() const;

		void setConsoleLog(const QString& log);
		QString getConsoleLog() const;

		void setHttpMethod(int method);
		int getHttpMethod() const;

		void setUserAgent(const QString& userAgent);
        QString getUserAgent() const;

		void sendPayload();

	private slots:
		void processResponse(QNetworkReply* reply);
        void onTimeout();

    signals:
        void httpMethodChanged(int);
        void timeoutChanged(int);  
        void urlChanged(QString);
        void contentTypeChanged(QString);
        void consoleLogChanged(QString);
        void userAgentChanged(QString);

    private:
		bool validateHttpStatus(int status) const;
		QUrl resolveUrl(const QString& rawUrl) const;

        QNetworkRequest prepareRequest();

		QNetworkReply* sendGet(QNetworkRequest request);
		QNetworkReply* sendPost(QNetworkRequest request);
		QNetworkReply* sendDel(QNetworkRequest request);
		QNetworkReply* sendPut(QNetworkRequest request);
		QNetworkReply* sendPatch(QNetworkRequest request);

    private:
        InputSlot* _inputSlot = nullptr;
        OutputSlot* _outputSlot = nullptr;
        int _timeout;
        int _httpMethod;
        QString _url;
        QString _contentType;
        QString _consoleLog;
        QString _userAgent;
        QNetworkAccessManager* _networkAccessManager = nullptr;
        QElapsedTimer _elapsedTimer;
        QTimer _timer;
        QNetworkReply* _networkReply = nullptr;
    };
}