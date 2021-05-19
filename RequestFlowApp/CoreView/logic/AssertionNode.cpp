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
    auto widget = new QWidget();
    _ui.setupUi(widget);
    getContentWidget()->layout()->addWidget(widget);

	auto node = getModelNode<model::AssertionNode*>();    
    _ui.textEdit->setPlainText(node->getScript());
    _ui.textEdit->setPlaceholderText("// javascript");

    connect(_ui.textEdit, &QTextEdit::textChanged, this, [=]()
		{
            node->setScript(_ui.textEdit->toPlainText());
		});

	_bgColor = view::colors::vividBurgundy;

	connect(_node, &model::Node::ready, this, [=]()
		{
			getModelNode()->evaluate();
		}, Qt::ConnectionType::QueuedConnection);

	setMinSize(QSize(300, 200));
	setSize(300, 200);
}
