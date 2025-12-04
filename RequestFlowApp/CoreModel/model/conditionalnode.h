#ifndef CONDITIONALNODE_H
#define CONDITIONALNODE_H

#include "Node.h"
#include "Message.h"
#include "Graph.h"
#include "Slot.h"

namespace model {
    class COREMODEL_EXPORT ConditionalNode : public model::Node
    {
        Q_OBJECT
        Q_PROPERTY(QString script MEMBER _condition READ getCondition WRITE setCondition);
    public:
        Q_INVOKABLE ConditionalNode(model::Graph* graph);

        void createModel() override;
        void evaluate() override;

        void setCondition(const QString& condition);
        QString getCondition() const;
    private:
        QString _condition;
    };
}

#endif // CONDITIONALNODE_H
