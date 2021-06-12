#include "MainWindow.h"
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
#include <view/Edge.h>
#include <QActionGroup>
#include "AppSettingDialog.h"

void MainWindow::setupMenuBar()
{
    // File menu
    {
        _fileMenu = menuBar()->addMenu("File");
        _newProjectAction = _fileMenu->addAction(QIcon(":/ui/new_file"), "New...", [=](){onNewProject();}, QKeySequence::New);
        _openProjectAction = _fileMenu->addAction(QIcon(":/ui/open_file"), "Open...", [=](){onOpenProject();}, QKeySequence::Open);
        //_openProjectAction->setMenu(new QMenu);

        _recentProjectsMenu = _fileMenu->addMenu("Recent projects...");
        updateRecentProjectsList();

        _saveProjectAction = _fileMenu->addAction(QIcon(":/ui/save_file"), "Save", [=](){onSaveProject();}, QKeySequence::Save);
        _saveProjectAsAction =  _fileMenu->addAction(QIcon(), "Save As...", [=](){onSaveProjectAs();}, QKeySequence::SaveAs);
        _fileMenu->addSeparator();
        _settingsAction = _fileMenu->addAction(QIcon(), "Settings...", [=]()
                                               {
                                                   auto dlg = AppSettingDialog(this);
                                                   dlg.exec();
                                               });
        //_settingsAction->setEnabled(false); // TODO: implement a settings dialog
        _fileMenu->addSeparator();
        _closeProjectAction = _fileMenu->addAction(QIcon(":/ui/close_file"), "Close Project", [=](){ onCloseProject(); });
        _fileMenu->addSeparator();
        _quitProjectAction = _fileMenu->addAction(QIcon(":/ui/close"), "Quit", [=](){close();}, QKeySequence::Quit);

        _saveProjectAction->setEnabled(false);
        _saveProjectAsAction->setEnabled(false);
    }

    // Edit menu
    {
        _editMenu = menuBar()->addMenu("Edit");
        _undoAction = _editMenu->addAction(QIcon(), "Undo", [](){}, QKeySequence::Undo);
        _redoAction = _editMenu->addAction(QIcon(), "Redo", [](){}, QKeySequence::Redo);

        _undoAction->setEnabled(false);
        _redoAction->setEnabled(false);

        _editMenu->addSeparator();
        _findNodeAction = _editMenu->addAction(QIcon(), "Find Node...", [=]()
            {
                auto sgw = getActiveSceneGraphWidget();
                if(sgw != nullptr)
                    sgw->findNodeDialog();
            }, QKeySequence::Find);
    }

    // Scenario menu
    {
        _scenariosMenu = menuBar()->addMenu("Scenario");
        _createScenarioAction = _scenariosMenu->addAction(QIcon(":/ui/test_case"), "Create Scenario...", this, [=]()
                                                          {
                                                              auto name = QInputDialog::getText(this, "Create Scenario", "Name :");
                                                              if (!name.isEmpty())
                                                              {
                                                                  createScenario(name);
                                                              }
                                                          });

        _cloneScenarioAction = _scenariosMenu->addAction(QIcon(":/ui/duplicate"), "Clone Scenario...", this, [=]()
                                                         {
                                                             cloneActiveScenario();
                                                         });

        _deleteScenarioAction = _scenariosMenu->addAction(QIcon(":/ui/minus"), "Delete Scenario", this, [=]()
                                                          {
                                                              deleteActiveScenario();
                                                          });
    }

    // Environment menu
    {
        _environmentsMenu = menuBar()->addMenu("Environment");
        _environmentConfigAction = _environmentsMenu->addAction(QIcon(), "Configure...", [=]()
                                                                {
                                                                    QDialog dialog(this);
                                                                    dialog.setWindowTitle("Environments");
                                                                    auto layout = new QVBoxLayout(&dialog);
                                                                    //layout->setMargin(0);
                                                                    layout->setSpacing(0);
                                                                    auto environmentsWidget = new EnvironmentsWidget(&dialog);
                                                                    layout->addWidget(environmentsWidget);
                                                                    environmentsWidget->setProject(_project.get());
                                                                    dialog.exec();
                                                                });
        _environmentsMenu->addSeparator();
        _importSwaggerAction = _environmentsMenu->addAction(QIcon(":/ui/swagger"), "Swagger Import...", [=](){onImportSwagger();});
    }

    // View menu
    {
        _viewMenu = menuBar()->addMenu("View");

        // TODO: implement messagelogger, inventory widgets menu items
        //auto messageLoggerWidgetAction = _viewMenu->addAction("Message Logger");
        //messageLoggerWidgetAction->setCheckable(true);
        //auto inventoryWidgetAction = _viewMenu->addAction("Inventory");
        //inventoryWidgetAction->setCheckable(true);
        //_viewMenu->addSeparator();

        _centerOnAction = _viewMenu->addAction(QIcon(), "Center on scene", [=](){});
        _centerOnAction->setEnabled(false);
        _viewMenu->addSeparator();
        _switchThemeAction = _viewMenu->addAction(QIcon(), "Switch theme...", [=]()
                                                  {
                                                      qDebug() << "clicked on " << _viewMenu;
                                                  });
        _switchThemeAction->setEnabled(false);

        _viewMenu->addSeparator();

        auto setScenesBg = [=](view::SceneGraph::BackgroundType bgType)
        {
            _settingsManager->setBackgroundType(bgType);
            for(auto subWindow : _ui.mdiArea->subWindowList())
            {
                auto sceneGraphWidget = dynamic_cast<SceneGraphWidget*>(subWindow->widget());
                if(sceneGraphWidget!=nullptr)
                {
                    sceneGraphWidget->getSceneGraph()->setBackgroundType(bgType);
                    sceneGraphWidget->getSceneGraph()->update();
                }
            }
        };

        auto setEdgesStyle = [=](view::SceneGraph::EdgeType edgeType)
        {
            _settingsManager->setEdgesStyle(edgeType);
            for(auto subWindow : _ui.mdiArea->subWindowList())
            {
                auto sceneGraphWidget = dynamic_cast<SceneGraphWidget*>(subWindow->widget());
                if(sceneGraphWidget!=nullptr)
                {
                    sceneGraphWidget->getSceneGraph()->setEdgeType(edgeType);
                    sceneGraphWidget->getSceneGraph()->update();
                }
            }
        };

        auto backgroundMenu = _viewMenu->addMenu("Background");
        auto solidColorAction = backgroundMenu->addAction(QIcon(), "Solid Color", [=]()
                                                          {
                                                              setScenesBg(view::SceneGraph::BackgroundType::SOLID);
                                                          });
        auto dotsAction = backgroundMenu->addAction(QIcon(), "Dots", [=]()
                                                    {
                                                        setScenesBg(view::SceneGraph::BackgroundType::DOTS);
                                                    });
        auto crossesAction = backgroundMenu->addAction(QIcon(), "Crosses", [=]()
                                                       {
                                                           setScenesBg(view::SceneGraph::BackgroundType::CROSSES);
                                                       });
        auto gridAction = backgroundMenu->addAction(QIcon(), "Grid", [=]()
                                                    {
                                                        setScenesBg(view::SceneGraph::BackgroundType::GRID);
                                                    });

        solidColorAction->setCheckable(true);
        dotsAction->setCheckable(true);
        crossesAction->setCheckable(true);
        gridAction->setCheckable(true);

        auto backgroundTypeActionGroup = new QActionGroup(backgroundMenu);
        backgroundTypeActionGroup->addAction(solidColorAction);
        backgroundTypeActionGroup->addAction(dotsAction);
        backgroundTypeActionGroup->addAction(crossesAction);
        backgroundTypeActionGroup->addAction(gridAction);

        switch(_settingsManager->getBackgroundType())
        {
        case 0:
            solidColorAction->setChecked(true);
            solidColorAction->trigger();
            break;
        case 1:
            dotsAction->setChecked(true);
            dotsAction->trigger();
            break;
        case 2:
            crossesAction->setChecked(true);
            crossesAction->trigger();
            break;
        case 3:
            gridAction->setChecked(true);
            gridAction->trigger();
            break;
        }

        auto edgeStyleMenu = _viewMenu->addMenu("Edge Style");
        auto curvesAction = edgeStyleMenu->addAction(QIcon(), "Curves", [=]()
                                                     {
                                                         setEdgesStyle(view::SceneGraph::EdgeType::CURVES);
                                                     });
        auto linesAction = edgeStyleMenu->addAction(QIcon(), "Lines", [=]()
                                                    {
                                                        setEdgesStyle(view::SceneGraph::EdgeType::LINES);
                                                    });

        curvesAction->setCheckable(true);
        linesAction->setCheckable(true);

        auto edgeTypeActionGroup = new QActionGroup(edgeStyleMenu);
        edgeTypeActionGroup->addAction(curvesAction);
        edgeTypeActionGroup->addAction(linesAction);
        switch(_settingsManager->getEdgesStyle())
        {
        case 0:
            curvesAction->setChecked(true);
            curvesAction->trigger();
            break;
        case 1:
            linesAction->setChecked(true);
            linesAction->trigger();
            break;
        }
    }

    // Help menu
    {
        _helpMenu = menuBar()->addMenu("Help");
        _helpMenu->addAction(QIcon(), "Contact Support...", [=](){onContactSupport();});
        _helpMenu->addAction(QIcon(), "RequestFlow website", [=](){onWebsite();});
        _helpMenu->addAction(QIcon(), "@requestflow on Twitter", [=](){onTwitter();});
        _helpMenu->addSeparator();
        auto activateAction = _helpMenu->addAction(QIcon(), "Activate", [=](){onActivateLicense();});
        activateAction->setEnabled(false); // TODO: create activation page
        _helpMenu->addSeparator();
        _helpMenu->addAction(QIcon(), "About", [=](){onAbout();});
    }
}

