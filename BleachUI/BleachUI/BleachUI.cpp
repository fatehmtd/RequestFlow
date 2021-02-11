#include "BleachUI.h"
#include "./view/SceneGraph.h"
#include "./view/SceneGraphWidget.h"

BleachUI::BleachUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setMinimumSize(800, 600);
    setCentralWidget(new SceneGraphWidget(this));
}
