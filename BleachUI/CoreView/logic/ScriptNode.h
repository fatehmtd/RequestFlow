#pragma once
#include "../view/Node.h"
#include <QPlainTextEdit>
#include <model/ScriptNode.h>
#include "ui_ScriptNodeUi.h"

namespace logic
{
	class ScriptNode : public view::Node
	{
		Q_OBJECT
	public:
		ScriptNode(model::ScriptNode* modelNode);
		virtual void clearUI() override;
	private:
		void setupUi();
	private:
		Ui::ScriptNodeUiWidget _ui;
	};
}
