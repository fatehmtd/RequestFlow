#pragma once
#include "../coremodel_global.h"
#include "Node.h"
#include "Message.h"
#include "Graph.h"
#include "Slot.h"

namespace model
{
    class COREMODEL_EXPORT PayloadNode : public model::Node
    {
        Q_OBJECT
        Q_PROPERTY(QVariant message READ messageToVariant WRITE messageFromVariant)
    public:
        Q_INVOKABLE PayloadNode(model::Graph* graph);
        void createModel() override;

        void setMessage(const model::Message& message);
        model::Message getMessage() const;

        void evaluate() override;
        OutputSlot* getOutputSlot() const;
    protected:
        QVariant messageToVariant() const;
        void messageFromVariant(const QVariant& v);
    private:
        model::Message _message;
    };
}