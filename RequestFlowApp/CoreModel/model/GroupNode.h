#pragma once
#include "Node.h"
#include <QColor>

namespace model
{
    class COREMODEL_EXPORT GroupNode : public Node
    {
        Q_OBJECT
        Q_PROPERTY(QColor color MEMBER _color READ getColor WRITE setColor NOTIFY colorChanged)
    public:
        GroupNode(model::Graph* graph);

        const QColor &getColor() const;
        void setColor(const QColor &newColor);

        void addNode(Node* node);
        void removeNode(Node* node);

        QList<Node*> getNodes() const;

    signals:
        void colorChanged();

        void nodeAdded(model::Node*);
        void nodeRemoved(QString);

        void contentChanged();

    private:
        QColor _color;
        QHash<QString, Node*> _nodes;
    };
}
