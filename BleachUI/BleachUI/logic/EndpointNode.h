#pragma once
#include "../view/Node.h"
#include "../view/Slot.h"
#include "ui_EndpointNodeUi.h"
#include <QTextEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QTimer>
#include <QElapsedTimer>

namespace logic
{
    class EndpointNode : public view::Node
    {
        Q_OBJECT
    public:
        Q_INVOKABLE EndpointNode(model::Node* modelNode);

        virtual QJSValue toJSValue(QJSEngine& engine) const;

        enum HttpMethod
        {
            GET,
            POST,
            PUT,
            PATCH,
            DEL
        };

        void setTimeout(unsigned int sec);
        void setUrl(const QUrl& url);
        void setMethod(HttpMethod method);

        virtual void clearUI() override;
    private:
        void initUI();
        void sendPayload();
        void sendGet();
        void sendPost();
        void sendDel();
        void sendPut();
        void sendPatch();
        bool validateHttpStatus(int status) const;
        QUrl resolveUrl(const QString& rawUrl) const;

        void processResponse(QNetworkReply* reply);

    private slots:
        void replyReceived(QNetworkReply* reply);

    private:
        Ui::EndpointNodeUiWidget* _ui = nullptr;
        QNetworkAccessManager* _networkAccessManager = nullptr;
        model::InputSlot* _input;
        model::OutputSlot* _output;
        QTimer _timer;
        QElapsedTimer _elapsedTimer;
    };
}