#include "externalnode.h"
#include "externalnodeselectiondialog.h"

#include <QFileDialog>

logic::ExternalNode::ExternalNode(model::ExternalNode *modelNode) : Node(modelNode, "External")
{
    setupUi();
}

void logic::ExternalNode::clearUI()
{

}

void logic::ExternalNode::setupUi()
{
    auto widget = new QWidget();
    _ui.setupUi(widget);

    getContentWidget()->layout()->addWidget(widget);

    auto node = getExternalNode();

    // fill the ui with node data
    switch(node->getExecutionCondition())
    {
    case model::ExternalNode::ExecutionCondition::ALWAYS:
        _ui.radioButton_alwaysExecute->setChecked(true);
        break;
    case model::ExternalNode::ExecutionCondition::RESULT_IS_OLD:
        _ui.radioButton_executeOlder->setChecked(true);
        break;
    case model::ExternalNode::ExecutionCondition::CUSTOM:
        _ui.radioButton_customScript->setChecked(true);
        break;
    }
    _ui.spinBox_delay->setValue(node->getDeltaDelay());
    _ui.textEdit_customScript->setPlainText(node->getCustomScript());

    auto linkedNode = node->getLinkedNode();
    if(linkedNode != nullptr)
    {
        setSelectedNode(linkedNode);
    }


    // connect the ui signals to the node
    connect(_ui.radioButton_alwaysExecute, &QAbstractButton::toggled, node, [=](bool status)
    {
        if(status)
            node->setExecutionCondition(model::ExternalNode::ExecutionCondition::ALWAYS);
    });

    connect(_ui.radioButton_executeOlder, &QAbstractButton::toggled, node, [=](bool status)
    {
        if(status)
            node->setExecutionCondition(model::ExternalNode::ExecutionCondition::RESULT_IS_OLD);
    });

    connect(_ui.radioButton_customScript, &QAbstractButton::toggled, node, [=](bool status)
    {
        if(status)
            node->setExecutionCondition(model::ExternalNode::ExecutionCondition::CUSTOM);
    });

    connect(_ui.textEdit_customScript, &QTextEdit::textChanged, node, [=]()
    {
        node->setCustomScript(_ui.textEdit_customScript->toPlainText());
    });

    connect(_ui.pushButton_browse, &QPushButton::clicked, this, &logic::ExternalNode::onBrowse);

    connect(_ui.spinBox_delay, SIGNAL(valueChanged(int)), this, SLOT(onDeltaDelayChanged(int)));

    connect(_node, &model::Node::ready, this, [=]()
    {
        node->evaluate();
    });

    ////////////////////////////////////////////////////////////////////////////////////////

    setMinSize(QSize(220, 200));
    setSize(220, 200);

    _bgColor = view::colors::lightGrey;
    //_ui.radioButton_alwaysExecute
}

model::ExternalNode *logic::ExternalNode::getExternalNode() const
{
    return dynamic_cast<model::ExternalNode*>(getModelNode());
}

void logic::ExternalNode::setSelectedNode(model::Node *linkedNode)
{
    _ui.lineEdit_externalGraph->setText(linkedNode->getGraph()->getName());
    _ui.lineEdit_externalNode->setText(QString("%1[%2]").arg(linkedNode->getType()).arg(linkedNode->getName()));
}

void logic::ExternalNode::onDeltaDelayChanged(int value)
{
    getExternalNode()->setDeltaDelay(value);
}

#include <QGraphicsView>
#include <QDebug>

void logic::ExternalNode::onBrowse()
{
    ExternalNodeSelectionDialog dlg(getModelNode()->getGraph(), scene()->views().first());
    if(dlg.exec() == 1) // accepted
    {
        auto linkedNode = dlg.getSelectedNode();
        if(linkedNode != nullptr)
        {
            getExternalNode()->setLinkedNode(linkedNode);
            setSelectedNode(linkedNode);
        }
    }
}
