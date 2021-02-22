#pragma once
#include "../view/Node.h"
#include <QTextEdit>

namespace logic
{
    class ViewerNode : public view::Node
    {
    public:
        ViewerNode(model::Node* modelNode);
    private:
        void setupUi();

    private:
        QTextEdit* _editor = nullptr;
    };
}