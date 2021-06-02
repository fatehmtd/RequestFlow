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
    _ui.plainTextEdit->setPlainText(node->getScript());
    _ui.plainTextEdit->setPlaceholderText("// javascript");

    connect(_ui.plainTextEdit, &QPlainTextEdit::textChanged, this, [=]()
		{
            node->setScript(_ui.plainTextEdit->toPlainText());
		});

    //_bgColor = view::colors::vividBurgundy;
    _bgColor = view::colors::nodes::assertion;

	connect(_node, &model::Node::ready, this, [=]()
		{
			getModelNode()->evaluate();
		}, Qt::ConnectionType::QueuedConnection);

	setMinSize(QSize(300, 200));
	setSize(300, 200);
}
