#pragma once
#include "../view/Node.h"
#include <model/AssertionNode.h>

namespace logic
{
    class AssertionNode : public view::Node
    {
    public:
        Q_INVOKABLE AssertionNode(model::Node* modelNode);

        virtual void clearUI() override;

    private:
        void setupUi();
    };
}
