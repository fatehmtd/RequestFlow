#pragma once
#include "../view/Node.h"
#include <model/AssertionNode.h>
#include <QPlainTextEdit>

namespace logic
{
    class AssertionNode : public view::Node
    {
    public:
        Q_INVOKABLE AssertionNode(model::AssertionNode* modelNode);

        virtual void clearUI() override;
    private:
        void setupUi();
    private:
        QPlainTextEdit* _editor = nullptr;
    };
}
