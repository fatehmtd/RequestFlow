#pragma once
#include "../view/Node.h"
#include "ui_DelayNodeUi.h"
#include <QTimer>

namespace logic
{
    class DelayNode : public view::Node
    {
    public:
        Q_INVOKABLE DelayNode(model::Node* modelNode);

        virtual void clearUI() override;

        virtual QJSValue toJSValue(QJSEngine& engine) const;
        virtual void fromJSValue(const QJSValue& jsValue);

    private slots:
        void onTimeout();
    private:
        void setupUi();
    private:
        Ui::DelayNodeUiForm _ui;
        QTimer* _timer;
    };
}
