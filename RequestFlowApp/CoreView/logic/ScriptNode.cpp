#include "ScriptNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Slot.h>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJSEngine>
#include <QJSValue>

logic::ScriptNode::ScriptNode(model::ScriptNode* modelNode) : view::Node(modelNode, "Script")
{
	setupUi();
}

void logic::ScriptNode::clearUI()
{
}

#include <QTextStream>
#include <QFile>

void logic::ScriptNode::setupUi()
{
	auto widget = new QWidget();
	_ui.setupUi(widget);

    // hide these for now.
    _ui.comboBox_externalEditors->hide();
    _ui.pushButton_externalEditor->hide();

	auto node = getModelNode<model::ScriptNode*>();
	_ui.plainTextEdit->setPlainText(node->getScript());
	_ui.plainTextEdit->setPlaceholderText("// javascript");
	
	// Set monospace font for code editor
	QFont font("Courier New", 10);
	font.setStyleHint(QFont::Monospace);
    _ui.plainTextEdit->setFont(font);
	
	// Enable JavaScript syntax highlighting
	_highlighter = new view::JSHighlighter(_ui.plainTextEdit->document());
	
	// Enable code completion (Ctrl+Space or auto-trigger after 2 chars)
    //_completer = new view::JSCompleter(_ui.plainTextEdit);
	
	getContentWidget()->layout()->addWidget(widget);

	connect(_ui.plainTextEdit, &QPlainTextEdit::textChanged, this, [=]()
		{
			node->setScript(_ui.plainTextEdit->toPlainText());
		});

    //_bgColor = view::colors::charcoal;
    _bgColor = view::colors::nodes::script;
    setSvgIcon((":/nodes/script"));

    connect(_node, &model::Node::ready, this, [=]()
        {
            getModelNode()->evaluate();
        }, Qt::ConnectionType::QueuedConnection);

    setMinSize(QSize(400, 300));
	setSize(300, 200);
}
