#pragma once
#include "../coremodel_global.h"
#include "Node.h"
#include "Message.h"
#include "Graph.h"
#include "Slot.h"

namespace model
{
    class COREMODEL_EXPORT AssertionNode : public model::Node
    {
        Q_OBJECT
        Q_PROPERTY(QString script MEMBER _script READ getScript WRITE setScript)
    public:
        Q_INVOKABLE AssertionNode(model::Graph* graph);
        
        void createModel() override;

        void setScript(const QString& script);
        QString getScript() const;

        void evaluate() override;

		InputSlot* getInputSlot() const;
		OutputSlot* getOutputSlot() const;
    private:
        QString _script;
    };
}
