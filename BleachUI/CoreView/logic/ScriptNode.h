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
		virtual void clearUI() override;
	private:
		void setupUi();
	private:
		QTextEdit* _editor = nullptr;
	};
}
