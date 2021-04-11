#pragma once
#include "../view/Node.h"
#include <QTextEdit>
#include <model/ScriptNode.h>

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
