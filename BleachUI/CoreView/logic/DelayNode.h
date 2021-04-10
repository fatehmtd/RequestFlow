#pragma once
#include "../view/Node.h"
#include "ui_DelayNodeUi.h"
#include <QTimer>

namespace model
{
    class DelayNode : public model::Node
    {
        Q_OBJECT
    public:
        DelayNode(model::Graph* graph);

        void createModel() override;

        unsigned int getDelay() const;

    public slots:
        void setDelay(int duration);

    private slots:
        void onTimeout();

    private:
        model::InputSlot* _inputSlot = nullptr;
        model::OutputSlot* _outputSlot = nullptr;
        QTimer _timer;
    };
}

namespace logic
{
    class DelayNode : public view::Node
    {
    public:
        DelayNode(model::DelayNode* modelNode);

        virtual void clearUI() override;

        virtual QJSValue toJSValue(QJSEngine& engine) const;
        virtual void fromJSValue(const QJSValue& jsValue);

    private:
        void setupUi();
    private:
        Ui::DelayNodeUiForm _ui;
    };
}
