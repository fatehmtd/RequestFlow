#include "conditionalnode.h"


model::ConditionalNode::ConditionalNode(model::Graph *graph): Node(graph, "Condition")
{

}

void model::ConditionalNode::createModel()
{
    for(int i=0;i<4;i++) {
        addInputSlot(QString("Input %d").arg(i), Slot::CUSTOM+5);
    }

    addOutputSlot("OutputSlot", Slot::CUSTOM+5);
}

void model::ConditionalNode::evaluate()
{
    Node::evaluate();
}

void model::ConditionalNode::setCondition(const QString &condition)
{
    _condition = condition;
}

QString model::ConditionalNode::getCondition() const
{
    return _condition;
}
