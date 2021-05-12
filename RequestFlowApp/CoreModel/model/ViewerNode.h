#pragma once
#include "../coremodel_global.h"
#include "Node.h"
#include "Message.h"
#include "Graph.h"
#include "Slot.h"

namespace model
{
    class Graph;

    class COREMODEL_EXPORT ViewerNode : public model::Node
    {
        Q_OBJECT
        Q_PROPERTY(QString filter MEMBER _filter READ getFilter WRITE setFilter)
    public:
        Q_INVOKABLE ViewerNode(model::Graph* graph);

        void createModel() override;

        model::InputSlot* getInput() const;

        void setFilter(const QString& filter);
        QString getFilter() const;
    private:
        QString _filter;
    };
}