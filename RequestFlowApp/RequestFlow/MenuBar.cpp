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

void MainWindow::setupMenuBar()
{
    auto bar= menuBar();

    // File menu
    {
        auto mainMenu = bar->addMenu("File");
        mainMenu->addAction(QIcon(), "New...", [=](){onNewProject();}, QKeySequence("Ctrl+N"));
        mainMenu->addAction(QIcon(), "Open...", [=](){onOpenProject();}, QKeySequence("Ctrl+O"));
        mainMenu->addAction(QIcon(), "Save", [=](){onOpenProject();}, QKeySequence("Ctrl+S"));
        mainMenu->addAction(QIcon(), "Save As...", [=](){onOpenProject();}, QKeySequence("Ctrl+Shift+S"));
        mainMenu->addSeparator();
        mainMenu->addAction(QIcon(), "Settings...", [=](){});
        mainMenu->addSeparator();
        mainMenu->addAction(QIcon(), "Quit", [=](){close();}, QKeySequence("Ctrl+Q"));
    }

    //bar->addMenu("View");

    // Scenario menu
    {
        bar->addMenu("Scenario");
    }

    // Environment menu
    {
        auto mainMenu = bar->addMenu("Environment");
        mainMenu->addAction(QIcon(), "Configure...", [=](){});
        mainMenu->addSeparator();
        mainMenu->addAction(QIcon(":/ui/swagger"), "Swagger Import...", [=](){onImportSwagger();});
    }

    // Window menu
    {
        auto mainMenu = bar->addMenu("Window");
        mainMenu->addAction(QIcon(), "Center on scene", [=](){});
        mainMenu->addSeparator();
        mainMenu->addAction(QIcon(), "Switch theme...", [=](){});
    }

    // Help menu
    {
        auto mainMenu = bar->addMenu("Help");
        mainMenu->addAction(QIcon(), "Contact Support...", [=](){onContactSupport();});
        mainMenu->addAction(QIcon(), "RequestFlow website", [=](){onWebsite();});
        mainMenu->addAction(QIcon(), "@requestflow on Twitter", [=](){onTwitter();});
        mainMenu->addSeparator();
        auto activateAction = mainMenu->addAction(QIcon(), "Activate", [=](){onActivateLicense();});
        activateAction->setEnabled(false); // TODO: create activation page
        mainMenu->addSeparator();
        mainMenu->addAction(QIcon(), "About", [=](){onAbout();});
    }
    //*/
}

MenuBar::MenuBar(QMainWindow *mainWindow) : QObject(mainWindow), _parent(mainWindow)
{

}
