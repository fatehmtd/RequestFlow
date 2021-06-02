#include "GroupNode.h"

model::GroupNode::GroupNode(Graph *graph) : Node(graph, "Group")
{

}

const QColor & model::GroupNode::getColor() const
{
    return _color;
}

void model::GroupNode::setColor(const QColor &newColor)
{
    if (_color == newColor)
        return;
    _color = newColor;
    emit colorChanged();
}

void model::GroupNode::addNode(Node *node)
{
    if(!_nodes.contains(node->getIdentifier()))
    {
        _nodes[node->getIdentifier()] = node;
        emit nodeAdded(node);
        emit contentChanged();
    }
}

void model::GroupNode::removeNode(Node *node)
{
    if(_nodes.contains(node->getIdentifier()))
    {
        _nodes.remove(node->getIdentifier());
        emit nodeRemoved(node->getIdentifier());
        emit contentChanged();
    }
}

QList<model::Node *> model::GroupNode::getNodes() const
{
    return _nodes.values();
}
