#pragma once
#include "../view/Node.h"
#include <QTextEdit>

namespace logic
{
    class ScriptNode : public view::Node
    {
    public:
        Q_INVOKABLE ScriptNode(model::Node* modelNode);

        virtual QJSValue toJSValue(QJSEngine& engine) const;

        virtual void clearUI() override;
    private:
        void setupUi();

        void executeScript();

    private:
        QTextEdit* _editor = nullptr;
    };
}
