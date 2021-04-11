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
	public:
		ScriptNode(model::Graph* graph);

		void setScript(const QString& script);
		QString getScript() const;

		InputSlot* getInputSlot() const;
		OutputSlot* getOutputSlot() const;

		void createModel() override;

		void evaluate() override;

	private:
		bool executeScript();		

	private:
		InputSlot* _inputSlot = nullptr;
		OutputSlot* _outputSlot = nullptr;
		QString _script;
	};
}