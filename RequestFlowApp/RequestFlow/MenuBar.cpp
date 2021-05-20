#include "MenuBar.h"
#include "./view/SceneGraph.h"
#include "./view/SceneGraphWidget.h"
#include <QDockWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QDebug>
#include <QObject>
#include <QPixmap>
#include <QPainter>
#include <QEvent>
#include <QWidget>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QMenuBar>


MenuBar::MenuBar(QMainWindow *mainWindow) : QObject(mainWindow), _parent(mainWindow)
{

}
