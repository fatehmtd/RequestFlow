#pragma once
#include "../view/Node.h"
#include <QTextEdit>

namespace model
{
	class ScriptNode : public model::Node
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

namespace logic
{
	class ScriptNode : public view::Node
	{
	public:
		ScriptNode(model::ScriptNode* modelNode);

		virtual QJSValue toJSValue(QJSEngine& engine) const;
		virtual void fromJSValue(const QJSValue& jsValue);

		virtual void clearUI() override;
	private:
		void setupUi();
	private:
		QTextEdit* _editor = nullptr;
	};
}
