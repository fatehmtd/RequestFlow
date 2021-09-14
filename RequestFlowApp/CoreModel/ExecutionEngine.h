#pragma once

#include "model/Graph.h"
#include "model/Message.h"
#include "model/Node.h"
#include <QObject>
#include <QQueue>

namespace model {

class ExecutionEngine : public model::IdentifiableEntity {
    Q_OBJECT
public:
    ExecutionEngine(model::Graph* graph);

    void stageNodeEvaluation(model::Node* node);

    model::Graph* getGraph() const;

signals:
    void nodeEvaluated(model::Node*, model::Message);
    void nodeStaged(model::Node*);

protected:
    virtual model::Message evaluateNode(model::Node* node) = 0;

protected:
    QQueue<model::Node*> _nodesQueue;
    model::Graph* _graph = nullptr;
};

}
