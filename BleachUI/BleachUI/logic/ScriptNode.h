#pragma once
#include "../view/Node.h"
#include <QTextEdit>

namespace logic
{
    class ScriptNode : public view::Node
    {
    public:
        ScriptNode(model::Node* modelNode);

        virtual void clearUI() override;
    private:
        void setupUi();

        void executeScript();

    private:
        QTextEdit* _editor = nullptr;
    };
}
