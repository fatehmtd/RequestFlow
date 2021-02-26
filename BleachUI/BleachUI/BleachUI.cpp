#include "BleachUI.h"
#include "./view/SceneGraph.h"

BleachUI::BleachUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setMinimumSize(1280, 800);
    _sceneGraphWidget = new SceneGraphWidget(this);
    setCentralWidget(_sceneGraphWidget);
}