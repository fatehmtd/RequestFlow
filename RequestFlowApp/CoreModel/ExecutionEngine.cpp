#include "ExecutionEngine.h"

model::ExecutionEngine::ExecutionEngine(model::Graph* graph)
    : model::IdentifiableEntity(graph)
{
}

void model::ExecutionEngine::stageNodeEvaluation(Node* node)
{
    _nodesQueue.enqueue(node);

    emit nodeStaged(node);
}
