#include "AssertionNode.h"

logic::AssertionNode::AssertionNode(model::AssertionNode* modelNode) : view::Node(modelNode, "Assertion")
{
    setupUi();
}

void logic::AssertionNode::clearUI()
{
}

void logic::AssertionNode::setupUi()
{
	auto node = getModelNode<model::AssertionNode*>();
	_editor = new QPlainTextEdit();
	_editor->setPlainText(node->getScript());
	_editor->setPlaceholderText("// javascript");
	getContentWidget()->layout()->addWidget(_editor);

	connect(_editor, &QPlainTextEdit::textChanged, this, [=]()
		{
			node->setScript(_editor->toPlainText());
		});

	_bgColor = view::colors::vividBurgundy;

	connect(_node, &model::Node::ready, this, [=]()
		{
			getModelNode()->evaluate();
		}, Qt::ConnectionType::QueuedConnection);

	setMinSize(QSize(300, 200));
	setSize(300, 200);
}
