#pragma once
#include "../coremodel_global.h"
#include "Node.h"
#include "Message.h"
#include "Graph.h"
#include "Slot.h"
#include <QTimer>

namespace model
{
    class COREMODEL_EXPORT DelayNode : public model::Node
    {
        Q_OBJECT
        Q_PROPERTY(int delay READ getDelay WRITE setDelay)
    public:
        Q_INVOKABLE DelayNode(model::Graph* graph);

        void createModel() override;

        int getDelay() const;

    public slots:
        void setDelay(int duration);

    private slots:
        void onTimeout();

    private:
        QTimer _timer;
    };
}