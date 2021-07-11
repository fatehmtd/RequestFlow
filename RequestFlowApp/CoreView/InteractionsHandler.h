#pragma once
#include "coreview_global.h"
#include <QGraphicsItem>
#include <QHash>
#include <QList>
#include <QMenu>
#include <QObject>
#include <functional>

namespace model {
class EndpointEntry;
}

namespace view {
class SceneGraph;
class Node;
class Slot;
class Edge;

class COREVIEW_EXPORT InteractionsHandler : public QObject {
public:
    InteractionsHandler(SceneGraph* scene);
    ~InteractionsHandler();

    typedef std::function<void(const QPointF&, QAction*)> ExecFunc;
    typedef std::function<bool(QGraphicsItem*)> FilterFunc;

    void registerGenericAction(const QString& name, std::function<bool(QGraphicsItem*)> filter, ExecFunc func, QIcon icon = QIcon(), int order = 99);
    void registerNodeTypeAction(const QString& name, const QString& nodeType, ExecFunc func, QIcon icon = QIcon(), int order = 99);
    void registerNodeAction(const QString& name, ExecFunc func, QIcon icon = QIcon(), int order = 99);
    void registerEdgeAction(const QString& name, ExecFunc func, QIcon icon = QIcon(), int order = 99);
    void registerEmptySpaceAction(const QString& name, ExecFunc func, QIcon icon = QIcon(), int order = 99);

    Node* createEndpointNode(const model::EndpointEntry* entry = nullptr);
    Node* createPayloadNode();
    Node* createViewerNode();
    Node* createDelayNode();
    Node* createScriptNode();
    Node* createAssertionNode();
    Node* createExternalNode();

    void deleteNode(Node* node);
    void deleteNodes(const QList<Node*>& nodes);
    void deleteEdge(Edge* edge);
    void deleteInputSlot(Slot* slot);

    QMenu* createContextMenu(const QPointF& p);

private:
    struct ItemAction {
        int order;
        QString name;
        QIcon icon;
        ExecFunc func;
        FilterFunc filter;
        //std::function<bool(void)> visibilityFilter;

        QHash<QString, ItemAction> _childrenMap;

        ItemAction()
        {
        }

        ItemAction(int order, QIcon icon, QString name, ExecFunc func, FilterFunc filter)
        {
            this->order = order;
            this->name = name;
            this->icon = icon;
            this->func = func;
            this->filter = filter;
        }

        ItemAction(const ItemAction& original)
        {
            this->order = original.order;
            this->name = original.name;
            this->icon = original.icon;
            this->func = original.func;
            this->filter = original.filter;
        }

        ItemAction& addSubAction(const ItemAction& action)
        {
            _childrenMap.insert(action.name, action);
            return _childrenMap[action.name];
        }

        bool operator<(const ItemAction& ia) const
        {
            return this->order >= ia.order;
        }
    };

    void registerCommonActions();
    void renameInputSlots(view::Node* node) const;

    QList<ItemAction> _itemActionsList;
    SceneGraph* _sceneGraph = nullptr;
};
}
