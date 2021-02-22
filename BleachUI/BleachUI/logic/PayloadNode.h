#pragma once
#include "../view/Node.h"
#include <QTextEdit>

namespace logic
{
    class PayloadNode : public view::Node
    {
    public:
        PayloadNode(model::Node* modelNode);
    private:
        void setupUi();

    private:
        QTextEdit* _editor = nullptr;
    };
}

