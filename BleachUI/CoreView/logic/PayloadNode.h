#pragma once
#include "../view/Node.h"
#include <QTextEdit>
#include "ui_PayloadNodeUi.h"
#include <model/PayloadNode.h>

namespace logic
{
    class PayloadNode : public view::Node
    {
    public:
        Q_INVOKABLE PayloadNode(model::PayloadNode* modelNode);

        virtual void clearUI() override;
    private:
        void setupUi();
        void prepareAndSend() const;
		model::Message composeMessage() const;
        void fillFromMessage(const model::Message& message);

        QMap<QString, QVariant> fillFromTable(QTableWidget* tableWidget) const;

    private:
        Ui::PayloadNodeUi _ui;
    };
}

