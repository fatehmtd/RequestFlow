#pragma once
#include "../view/Node.h"
#include <QTextEdit>

namespace logic
{
    class AssertionNode : public view::Node
    {
    public:
        Q_INVOKABLE AssertionNode(model::Node* modelNode);

        virtual void clearUI() override;

        virtual QJSValue toJSValue(QJSEngine& engine) const;
        virtual void fromJSValue(const QJSValue& jsValue);

    private:
        void setupUi();
    };
}
