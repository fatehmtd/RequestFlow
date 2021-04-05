#pragma once
#include "../view/Node.h"
#include <QTextEdit>
#include "ui_PayloadNodeUi.h"

namespace logic
{
    class PayloadNode : public view::Node
    {
    public:
        Q_INVOKABLE PayloadNode(model::Node* modelNode);

        QJSValue toJSValue(QJSEngine& engine) const override;
        void fromJSValue(const QJSValue& jsValue) override;

        model::Message composeMessage() const;
        virtual void clearUI() override;
    private:
        void setupUi();
        void prepareAndSend() const;
        QMap<QString, QVariant> fillFromTable(QTableWidget* tableWidget) const;

    private:
        Ui::PayloadNodeUi _ui;
    };
}

