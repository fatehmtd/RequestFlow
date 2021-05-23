#include "ViewerNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Slot.h>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QAbstractItemModel>
#include <QJSValue>
#include <QJSEngine>
#include <QFileDialog>
#include <QGraphicsView>
#include "../viewernodewidget.h"
#include <QDialog>

logic::ViewerNode::ViewerNode(model::ViewerNode* modelNode) : view::Node(modelNode, "Viewer")
{
	setupUi();
}

void logic::ViewerNode::clearUI()
{
    _largePayloadInfoWidget->setVisible(false);
    _viewWidget->setVisible(true);
    _viewWidget->clear();
}

#include <QDebug>

void logic::ViewerNode::setupUi()
{
    connect(_node, &model::Node::ready, this, [=]()
        {
            auto message = _node->getInputSlotsMap().first()->getData();
            auto data = message.getBody();
            bool isLarge = data.size() >= 9999999999999999999;
            _viewWidget->setVisible(!isLarge);
            _largePayloadInfoWidget->setVisible(isLarge);
        });

    _viewWidget = new ViewerNodeWidget(this, nullptr);
    _largePayloadInfoWidget = new LargePayloadInfoWidget(nullptr);
    getContentWidget()->layout()->addWidget(_largePayloadInfoWidget);
    getContentWidget()->layout()->addWidget(_viewWidget);

    connect(_largePayloadInfoWidget, &LargePayloadInfoWidget::viewClicked, this, [=]()
            {
                showDialog();
            });

    _largePayloadInfoWidget->setVisible(false);
	_bgColor = view::colors::green;

    setMinSize(QSize(400, 300));
    setSize(300, 200);
}

#include <QVBoxLayout>

void logic::ViewerNode::showDialog()
{
    QDialog dialog(nullptr);
    auto layout = new QVBoxLayout();
    dialog.setLayout(layout);

    auto viewWidget = new ViewerNodeWidget(this, nullptr);
    viewWidget->setMessage(_node->getInputSlots().first()->getData());
    viewWidget->fillData();
    layout->addWidget(viewWidget);
    dialog.exec();
    viewWidget->deleteLater();
}

