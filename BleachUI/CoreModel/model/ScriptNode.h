#pragma once
#include "../coremodel_global.h"
#include "Node.h"
#include "Message.h"
#include "Graph.h"
#include "Slot.h"

namespace model
{
	class COREMODEL_EXPORT ScriptNode : public model::Node
	{
		Q_OBJECT
		Q_PROPERTY(QString script MEMBER _script READ getScript() WRITE setScript)
	public:
		Q_INVOKABLE ScriptNode(model::Graph* graph);

		void setScript(const QString& script);
		QString getScript() const;

		InputSlot* getInputSlot() const;
		OutputSlot* getOutputSlot() const;

		void createModel() override;

		void evaluate() override;

	private:
		bool executeScript();		

	private:
		QString _script;
	};
}