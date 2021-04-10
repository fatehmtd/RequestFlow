#pragma once
#include "../view/Node.h"
#include <QTextEdit>
#include "ui_PayloadNodeUi.h"

namespace model
{
    class PayloadNode : public model::Node
    {
        Q_OBJECT
    public:
        PayloadNode(model::Graph* graph);
        void createModel() override;

        void setMessage(const model::Message& message);
        model::Message getMessage() const;

        void evaluate() override;
        OutputSlot* getOutputSlot() const;
    private:
        model::Message _message;
        OutputSlot* _outputSlot = nullptr;
    };
}

namespace logic
{
    class PayloadNode : public view::Node
    {
    public:
        Q_INVOKABLE PayloadNode(model::PayloadNode* modelNode);

        QJSValue toJSValue(QJSEngine& engine) const override;
        void fromJSValue(const QJSValue& jsValue) override;

        virtual void clearUI() override;
    private:
        void setupUi();
        void prepareAndSend() const;
		model::Message composeMessage() const;

        QMap<QString, QVariant> fillFromTable(QTableWidget* tableWidget) const;

    private:
        Ui::PayloadNodeUi _ui;
    };
}

