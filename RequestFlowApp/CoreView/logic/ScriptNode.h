#pragma once
#include "../view/Node.h"
#include <QPlainTextEdit>
#include <model/ScriptNode.h>
#include "ui_ScriptNodeUi.h"
#include "../JSHighlighter.h"
#include "../JSCompleter.h"

namespace logic
{
	class COREVIEW_EXPORT ScriptNode : public view::Node
	{
		Q_OBJECT
	public:
		ScriptNode(model::ScriptNode* modelNode);
		virtual void clearUI() override;
	private:
        void setupUi();
	private:
		Ui::ScriptNodeUiWidget _ui;
        view::JSHighlighter* _highlighter = nullptr;
        view::JSCompleter* _completer = nullptr;
	};
}
