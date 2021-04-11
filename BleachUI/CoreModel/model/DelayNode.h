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
    public:
        DelayNode(model::Graph* graph);

        void createModel() override;

        unsigned int getDelay() const;

    public slots:
        void setDelay(int duration);

    private slots:
        void onTimeout();

    private:
        model::InputSlot* _inputSlot = nullptr;
        model::OutputSlot* _outputSlot = nullptr;
        QTimer _timer;
    };
}