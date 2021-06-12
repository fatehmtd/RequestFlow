#pragma once

#include "Graph.h"
#include "Message.h"
#include "Node.h"
#include "Slot.h"
#include <chrono>

namespace model {
class COREMODEL_EXPORT ExternalNode : public model::Node
{
    Q_OBJECT
    Q_PROPERTY(QString customScript MEMBER _customScript READ getCustomScript
                   WRITE setCustomScript)
    Q_PROPERTY(int executionCondition MEMBER _executionCondition READ
                   getExecutionCondition WRITE setExecutionCondition)
    Q_PROPERTY(quint64 deltaDelay MEMBER _deltaDelay READ getDeltaDelay WRITE
                   setDeltaDelay)
    Q_PROPERTY(QString externalGraphId MEMBER _externalGraphId READ
                   getExternalGraphId WRITE setExternalGraphId)
    Q_PROPERTY(QString externalNodeId MEMBER _externalNodeId READ
                   getExternalNodeId WRITE setExternalNodeId)
public:
    Q_INVOKABLE ExternalNode(model::Graph* graph);

    enum ExecutionCondition
    {
        ALWAYS,
        RESULT_IS_OLD,
        CUSTOM
    };

    void createModel() override;

    void evaluate() override;

    OutputSlot* getOutputSlot() const;

    void setExecutionCondition(int condition);
    int getExecutionCondition() const;

    void setLinkedNode(Node* node);
    Node* getLinkedNode() const;

    void setDeltaDelay(quint64 delay);
    quint64 getDeltaDelay() const;

    void setCustomScript(const QString& script);
    QString getCustomScript() const;

    QString getExternalGraphId() const;
    QString getExternalNodeId() const;

    void prepareNodeInternals() override;

protected slots:
    void onExternalNodeDataReceived();

private:
    bool shouldRunExternal() const;
    bool checkDelay() const;
    bool evalScript() const;
    void setExternalGraphId(const QString& id);
    void setExternalNodeId(const QString& id);

private:
    Node* _linkedNode = nullptr;
    int _executionCondition;
    quint64 _deltaDelay;
    QString _customScript;
    QString _externalGraphId, _externalNodeId;
    bool _firstTimeDataSet = false;
    std::chrono::steady_clock::time_point _lastExecutionTimePoint;
};
} // namespace model
