#include "DelayNode.h"
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <model/Node.h>
#include <model/Slot.h>

logic::DelayNode::DelayNode(model::DelayNode *modelNode)
    : view::Node(modelNode, "Delay") {
  setupUi();
  _ui.spinBox->setValue(modelNode->getDelay());
  connect(_ui.spinBox, SIGNAL(valueChanged(int)), modelNode,
          SLOT(setDelay(int)));
}

void logic::DelayNode::clearUI() {
  //_editor->clear();
}

void logic::DelayNode::setupUi() {
  auto widget = new QWidget();
  _ui.setupUi(widget);

  getContentWidget()->layout()->addWidget(widget);

  setResizable(false);

  setMinSize(QSize(250, 150));
  setSize(100, 50);

  //_bgColor = view::colors::yellow;
  _bgColor = view::colors::nodes::delay;
  setSvgIcon((":/nodes/delay"));
}
