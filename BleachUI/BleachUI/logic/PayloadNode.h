#pragma once
#include "../view/Node.h"
#include <QTextEdit>
#include "ui_PayloadNodeUi.h"

namespace logic
{
    class PayloadNode : public view::Node
    {
    public:
        PayloadNode(model::Node* modelNode);

        virtual void clearUI() override;
    private:
        void setupUi();
        void prepareAndSend() const;
        QMap<QString, QVariant> fillFromTable(QTableWidget* tableWidget) const;

    private:
        Ui::PayloadNodeUi _ui;
    };
}

