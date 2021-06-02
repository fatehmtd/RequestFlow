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
        Q_PROPERTY(int authMethod MEMBER _authMethod READ getAuthMethod WRITE setAuthMethod)
        Q_PROPERTY(QString contentType MEMBER _contentType READ getContentType WRITE setContentType NOTIFY contentTypeChanged)
        Q_PROPERTY(QString bearerToken MEMBER _bearerToken READ getBearerToken WRITE setBearerToken)
        Q_PROPERTY(QString baUser MEMBER _baUser READ getBasicAuthUser WRITE setBasicAuthUser)
        Q_PROPERTY(QString baPwd MEMBER _baPwd READ getBasicAuthPassword WRITE setBasicAuthPassword)
        //Q_PROPERTY(QString consoleLog MEMBER _consoleLog READ getConsoleLog WRITE setConsoleLog NOTIFY consoleLogChanged)
        Q_PROPERTY(QString userAgent MEMBER _userAgent READ getUserAgent WRITE setUserAgent NOTIFY userAgentChanged)
        Q_PROPERTY(QString expectedPayload MEMBER _expectedPayload READ getExpectedPayload WRITE setExpectedPayload)
        //Q_PROPERTY(QMap<QString, QString> extraHeaders MEMBER _extraHeaders READ getExtraHeaders WRITE setExtraHeaders)
        //Q_PROPERTY(QList<unsigned int> acceptedCodes MEMBER _acceptedCodes READ getAcceptedCodes WRITE setAcceptedCodes)
        //Q_PROPERTY(QList<unsigned int> rejectedCodes MEMBER _rejectedCodes READ getRejectedCodes WRITE setRejectedCodes)
    public:
        Q_INVOKABLE EndpointNode(model::Graph* graph);
        ~EndpointNode();
        
        void createModel() override;

		enum HttpMethod
		{
			GET,
			POST,
			PUT,
            DEL,
            PATCH
		};
        Q_ENUM(HttpMethod);

        static int HttpMethodFromString(const QString& string);

        enum AuthorizationMethod
        {
            NONE,
            BASIC_AUTH,
            BEARER,
            OAUTH
        };
        Q_ENUM(AuthorizationMethod);

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

        void setExpectedPayload(QString& format);
        QString getExpectedPayload() const;

        void setAcceptedCodes(const QList<unsigned int>& codes);
        QList<unsigned int> getAcceptedCodes() const;

        void setRejectedCodes(const QList<unsigned int>& codes);
        QList<unsigned int> getRejectedCodes() const;

        void setAuthMethod(int method);
        int getAuthMethod() const;

        void setBasicAuthUser(const QString& user);
        void setBasicAuthPassword(const QString& pwd);

        QString getBasicAuthUser() const;
        QString getBasicAuthPassword() const;

        void setBearerToken(const QString& token);
        QString getBearerToken() const;

        // TODO: save and load extra headers
        void setExtraHeaders(const QMap<QString, QString>& headers);
        QMap<QString, QString> getExtraHeaders() const;
        QMap<QString, QString>& getExtraHeaders();

        QJSValue saveToJSValue(PersistenceHandler* handler) const override;
        bool loadFromJSValue(const QJSValue& v) override;
        
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

        void onErrorOccurred(QNetworkReply::NetworkError error);

        void onGraphStop() override;

    private:
        int _timeout;
        int _httpMethod;

        int _authMethod;
        QString _baUser, _baPwd;
        QString _bearerToken;

        QString _expectedPayload;

        QString _url;
        QString _contentType;
        QString _consoleLog;
        QString _userAgent;
        QList<unsigned int> _acceptedCodes, _rejectedCodes;
        QNetworkAccessManager* _networkAccessManager = nullptr;
        QElapsedTimer _elapsedTimer;
        QTimer _timer;
        QNetworkReply* _networkReply = nullptr;

        QMap<QString, QString> _extraHeaders;
    };
}
