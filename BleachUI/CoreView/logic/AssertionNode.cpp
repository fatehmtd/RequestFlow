#include "AssertionNode.h"

logic::AssertionNode::AssertionNode(model::Node* modelNode) : view::Node(modelNode, "Assertion")
{
    setupUi();
}

void logic::AssertionNode::clearUI()
{
}

QJSValue logic::AssertionNode::toJSValue(QJSEngine& engine) const
{
    auto value = Node::toJSValue(engine);
    return value;
}

void logic::AssertionNode::fromJSValue(const QJSValue& jsValue)
{
}

void logic::AssertionNode::setupUi()
{
}
