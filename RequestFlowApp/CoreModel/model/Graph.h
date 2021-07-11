#pragma once
#include "IdentifiableEntity.h"
#include <QElapsedTimer>
#include <QList>
#include <QMap>
#include <QObject>
#include <QVariant>
#include <chrono>

namespace model {
class Node;
class Edge;
class Slot;
class OutputSlot;
class InputSlot;
class Environment;
class Project;
class MessageLogger;

class COREMODEL_EXPORT Graph : public IdentifiableEntity {
    Q_OBJECT
public:
    Graph(Project* parent);
    ~Graph();

    Project* getProject() const;

    Graph* clone() const;

    QList<Node*> getNodes() const;
    QList<Edge*> getEdges() const;

    QList<Node*> getNodeParents(Node* node) const;
    bool checkIsParent(Node* child, Node* parent) const;

    Edge* findEdge(const InputSlot* destination, const OutputSlot* origin) const;
    QList<Edge*> findEdges(const Slot* slot) const;
    QList<Edge*> findEdges(const Node* node) const;

    Edge* connectSlots(OutputSlot* origin, InputSlot* destination);
    Edge* connectSlots(InputSlot* destination, OutputSlot* origin);
    bool canConnectSlots(Slot* origin, Slot* destination) const;

    enum ErrorCode {
        OK,
        MISSING_STARTING_NODE,
        MISSING_ENDING_NODE,
        MISSING_EDGES,
        ALREADY_RUNNING
    };

    enum State {
        IDLE,
        STARTED,
        SUCCEEDED,
        CANCELED,
        FAILED,
        STOPPED
    };

    bool isRunning() const;
    bool wasCanceled() const;
    bool hasFailed() const;
    bool hasSucceeded();

    void setActiveEnvironment(Environment* env);
    Environment* getActiveEnvironment() const;

    QJSValue saveToJSValue(PersistenceHandler* persistenceHandler) const override;
    bool loadFromJSValue(const QJSValue& v) override;
    Node* createNodeFromJSValue(const QJSValue& v);

    MessageLogger* getLogger() const;

    void prepareNodesInternals();

public slots:
    virtual int start();
    virtual void cancel();
    virtual void onNodeEvaluated();
    virtual void onNodeReady();
    virtual void onNodeFailed(const QString& reason);
    virtual void onNodeException(QString reason);
signals:
    void preparingStartup();
    void started();
    void stopped();
    void canceled();
    void failed(Node* node, QString reason);
    void failed();
    void succeeded();

    void advanced();
    void activeEnvironmentChanged();

private:
    int computeExecutionPath();
    void clear();
    void checkExecutionStatus();

private:
    QList<Node*> _startingNodes, _endingNodes;
    QMap<Node*, int> _executionNodes;
    QMap<Node*, std::chrono::system_clock::time_point> _executionTimes;
    Environment* _environment = nullptr;
    MessageLogger* _messageLogger = nullptr;
    QElapsedTimer _elapsedTimer;
    bool _isRunning = false;
    State _currentState, _finalExecutionState;
};
}
