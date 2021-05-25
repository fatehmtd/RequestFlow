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
#include "BackgroundPaintFilter.h"
#include <view/Node.h>
#include <view/SceneGraph.h>
#include <QFile>
#include <QTextStream>
#include <QJSValueIterator>
#include <model/Document.h>
#include "view/Node.h"
#include <QPropertyAnimation>
#include "aboutwidget.h"
#include <QDialog>
#include <QSizePolicy>
#include <QDesktopServices>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QDialog>


class CloseEventIgnoreEventFilter : public QObject
{
public:
    using QObject::QObject;

    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if(event->type() == QEvent::Type::Close)
        {
            event->ignore();
            return true;
        }
        return QObject::eventFilter(watched, event);
    }
};

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	_settingsManager = new view::SettingsManager(this);
	setupUi();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (onCloseProject() != -1)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::setupUi()
{
    _ui.setupUi(this);
	setWindowIcon(QIcon(":/ui/network"));
	//setMinimumSize(1280, 800);
    setupMenuBar();
    setupRibbonBar();
	setupSceneGraph();
	setupEnvironmentsWidget();
    setupCentralTitleBar();

	// enable the background image
    _ui.mdiArea->viewport()->installEventFilter(new BackgroundPaintFilter(this));
    //openLastProject();

    setProject(nullptr);
}

void MainWindow::setupRibbonBar()
{
	_ui.dockWidget->setTitleBarWidget(new QWidget());
    _ui.dockWidget->setFixedHeight(64);
    //_ui.dockWidget->setVisible(false);
}

void MainWindow::setupEnvironmentsWidget()
{
    _environmentsWidget = new EnvironmentsWidget();
}

void MainWindow::setupSceneGraph()
{
	_ui.mdiArea->setTabsClosable(false);
    connect(_ui.mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::onSubWindowActivated);

	connect(_ui.logMessagesWidget, &LogMessagesWidget::senderSelected, this, [=](model::Node* node)
		{
			for (auto subWindow : _ui.mdiArea->subWindowList())
			{
				auto sceneGraphWidget = dynamic_cast<SceneGraphWidget*>(subWindow->widget());
				if (sceneGraphWidget != nullptr)
				{
					if (sceneGraphWidget->getSceneGraph()->getModelGraph() == node->getGraph())
					{
						auto nodeGr = sceneGraphWidget->getSceneGraph()->findbyModel(node);

						_ui.mdiArea->setActiveSubWindow(subWindow);

						{
							auto propAnimation = new QPropertyAnimation();
							propAnimation->setTargetObject(sceneGraphWidget);
							propAnimation->setDuration(200);
							propAnimation->setEasingCurve(QEasingCurve(QEasingCurve::Type::InOutCubic));
							propAnimation->setStartValue(sceneGraphWidget->getCenter());
							propAnimation->setEndValue(nodeGr->pos() + QPointF(nodeGr->width() * 0.5f, nodeGr->height() * 0.5f));
							propAnimation->start(QAbstractAnimation::DeleteWhenStopped);
							sceneGraphWidget->setZoomLevel(2);
							connect(propAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& v)
								{
									sceneGraphWidget->centerOn(v.toPointF());
								});
						}
					}
				}
            }
            });
}

void MainWindow::setupCentralTitleBar()
{
    QPixmap pixmap(":/ui/test_case");
    _ui.label_scenarioIcon->setPixmap(pixmap.scaledToWidth(24));
    //_ui.label_scenarioIcon->setVisible(false);
    //_ui.toolButton_clone->setVisible(false);
    //_ui.toolButton_delete->setVisible(false);
    _ui.dockWidget->setVisible(false);
}

void MainWindow::openProject(const QString& fileName)
{
	if (!fileName.isEmpty())
	{
		QFile fp(fileName);
		if (fp.open(QIODevice::ReadOnly))
		{
			model::PersistenceHandler handler;
			QString contents(QString("(%1)").arg(QString(QJsonDocument::fromJson(fp.readAll()).toJson(QJsonDocument::JsonFormat::Compact))));

			auto projectValue = handler.evaluate(contents);

			if (!projectValue.isError())
			{
				onCloseProject();

				auto project = new model::Project(nullptr);
				project->setPath(fileName);
				project->loadFromJSValue(projectValue);

				setWindowTitle(QString("RequestFlow - %1").arg(fileName));

				setProject(project);

				loadFromJSValue(projectValue.property("ui"));

				_settingsManager->addRecentProject(fileName);
			}

			updateRecentProjectsList();
		}
	}
}

void MainWindow::setProject(model::Project* project)
{
	bool projectAvailable = project != nullptr;
    _ui.dockWidgetContents->setEnabled(projectAvailable);

    _environmentsWidget->setEnabled(projectAvailable);
    _ui.inventoryWidget->setEnabled(projectAvailable);
    _ui.dockWidget_4->setVisible(projectAvailable);
    _ui.dockWidget_5->setVisible(projectAvailable);

    // fill default objects if the project is empty
	if (projectAvailable)
	{
		_project.reset(project);

		if (project->getEnvironments().isEmpty())
		{
			auto environment = new model::Environment(project);
			environment->setName(QString("Default environment"));
			environment->getEntries().insert("baseUrl", "http://localhost");
		}

		if (project->getGraphs().isEmpty())
		{
			auto graph = new model::Graph(_project.get());
			graph->setName("Default scenario");
			auto sceneGraph = new view::SceneGraph(graph);
			openScenario(sceneGraph);
		}
		else
		{
			for (auto modelGraph : _project->getGraphs())
			{
				auto sceneGraph = new view::SceneGraph(modelGraph);
				openScenario(sceneGraph);
			}
		}
	}

    // File
    _saveProjectAction->setEnabled(projectAvailable);
    _saveProjectAsAction->setEnabled(projectAvailable);
    _closeProjectAction->setEnabled(projectAvailable);

    // View
    _viewMenu->setEnabled(projectAvailable);

    // Scenarios
    _scenariosMenu->setEnabled(projectAvailable);

    // Environments
    _environmentsMenu->setEnabled(projectAvailable);

	_ui.inventoryWidget->setProject(project);
	_ui.logMessagesWidget->setProject(project);
    _environmentsWidget->setProject(project);
}


void MainWindow::onAbout()
{
    auto dialog = new QDialog(this);
    dialog->setWindowFlag(Qt::WindowType::WindowCloseButtonHint, true);
    dialog->setWindowFlag(Qt::WindowType::WindowMinMaxButtonsHint, false);
    dialog->setWindowFlag(Qt::WindowType::WindowContextHelpButtonHint, false);
    dialog->setWindowFlag(Qt::WindowType::WindowTitleHint, false);
    //dialog->setSizePolicy(QSizePolicy::Policy::Fixed);
    auto widget = new AboutWidget(dialog);
    dialog->setFixedSize(widget->size());
    dialog->exec();
}

void MainWindow::onContactSupport()
{
    QDesktopServices::openUrl(QUrl("mailto:fateh@requestflow.dev?subject=Regarding RequestFlow"));
}

void MainWindow::onWebsite()
{
    QDesktopServices::openUrl(QUrl("http://www.requestflow.dev"));
}

void MainWindow::onActivateLicense()
{

}

void MainWindow::onTwitter()
{
    QDesktopServices::openUrl(QUrl("https://twitter.com/requestflow"));
}

void MainWindow::createScenario(QString name)
{
	auto graph = new model::Graph(_project.get());
	graph->setName(name);
    openScenario(new view::SceneGraph(graph));
	_ui.logMessagesWidget->addMessageLogger(graph->getLogger());
}

#include <model/Environment.h>

SceneGraphWidget* MainWindow::openScenario(view::SceneGraph* sceneGraph)
{
	auto sceneGraphWidget = new SceneGraphWidget(this, sceneGraph);

	sceneGraphWidget->setAttribute(Qt::WA_DeleteOnClose, false);
	sceneGraphWidget->setWindowFlag(Qt::WindowType::WindowCloseButtonHint, false);

    sceneGraph->setBackgroundType((view::SceneGraph::BackgroundType)_settingsManager->getBackgroundType());
    sceneGraph->setEdgeType((view::SceneGraph::EdgeType)_settingsManager->getEdgesStyle());

	sceneGraphWidget->setWindowIcon(QIcon(":/ui/test_case"));
	auto window = _ui.mdiArea->addSubWindow(sceneGraphWidget);

    auto evaluateScenarioTitle = [](model::Graph* graph)
    {
        QString envName = "No Environment Selected";
        if(graph->getActiveEnvironment()!=nullptr)
        {
            envName = graph->getActiveEnvironment()->getName();
        }
        return QString("%1 [Env: %2]").arg(graph->getName()).arg(envName);
    };

    connect(window, &QMdiSubWindow::aboutToActivate, this, [=]()
            {
                _ui.label_scenarioName->setText(evaluateScenarioTitle(sceneGraph->getModelGraph()));
                window->setWindowTitle(evaluateScenarioTitle(sceneGraph->getModelGraph()));
            });

	_subwindowsMap.insert(sceneGraph->getModelGraph()->getIdentifier(), window);

    window->installEventFilter(new CloseEventIgnoreEventFilter());

    //window->setAttribute(Qt::WA_DeleteOnClose, false);
    window->setWindowFlag(Qt::WindowType::WindowCloseButtonHint, true);
    window->setWindowFlag(Qt::WindowType::WindowShadeButtonHint, true);
	window->setWindowIcon(QIcon(":/ui/test_case"));
	window->setWindowTitle(sceneGraph->getModelGraph()->getName());

    auto sysMenu = new QMenu(window);
    sysMenu->addAction(QIcon(":/ui/duplicate"), "Clone Scenario", [=]()
                       {
                           auto newName = QInputDialog::getText(this, "Clone Scenario", "New Scenario Name : ", QLineEdit::Normal, QString("%1 - Clone").arg(sceneGraphWidget->getSceneGraph()->getModelGraph()->getName()));
                           if(!newName.isEmpty())
                           {
                               cloneScenario(sceneGraphWidget->getSceneGraph(), newName);
                           }
                       });

    sysMenu->addAction(QIcon(":/ui/delete"), "Delete Scenario", [=]()
                       {
                            deleteScenario(sceneGraphWidget->getSceneGraph());
                       });
    window->setSystemMenu(sysMenu);
    //window->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);

    connect(sceneGraph->getModelGraph(), &model::IdentifiableEntity::nameChanged, window, [=](const QString& str)
            {
                _ui.label_scenarioName->setText(evaluateScenarioTitle(sceneGraph->getModelGraph()));
                window->setWindowTitle(evaluateScenarioTitle(sceneGraph->getModelGraph()));
            });

    connect(sceneGraph->getModelGraph(), &model::Graph::activeEnvironmentChanged, window, [=]()
            {
                _ui.label_scenarioName->setText(evaluateScenarioTitle(sceneGraph->getModelGraph()));
                window->setWindowTitle(evaluateScenarioTitle(sceneGraph->getModelGraph()));
            });
	window->showMaximized();

    return sceneGraphWidget;
}

void MainWindow::cloneScenario(view::SceneGraph* sceneGraph, QString newName)
{
    auto sgw = (SceneGraphWidget*)sceneGraph->views()[0];
    model::PersistenceHandler persistenceHandler;
    auto jsValue = sgw->saveToJSValue(&persistenceHandler);
    auto newGraph = sceneGraph->getModelGraph()->clone();
    newGraph->setName(newName);
    auto newSceneGraph = new view::SceneGraph(newGraph);
    auto newSGW = openScenario(newSceneGraph);
    newSGW->loadFromJSValue(jsValue);
}

void MainWindow::deleteScenario(view::SceneGraph* sceneGraph)
{
    auto choice = QMessageBox::warning(this, "Delete Scenarion", QString("You are about to delete (%1), proceed ?").arg(sceneGraph->getModelGraph()->getName()), QMessageBox::Yes, QMessageBox::Cancel);
    if(choice == QMessageBox::Yes)
    {
        delete sceneGraph->getModelGraph();
        auto graphWidget = sceneGraph->views().first();
        delete graphWidget->parent();
    }
}

void MainWindow::deleteActiveScenario()
{

    //for(auto subWindow : _ui.mdiArea->subWindowList())
    {
        auto sceneGraphWidget = dynamic_cast<SceneGraphWidget*>(_ui.mdiArea->activeSubWindow()->widget());
        if(sceneGraphWidget != nullptr)
        {
            deleteScenario(sceneGraphWidget->getSceneGraph());
        }
    }
}

void MainWindow::cloneActiveScenario()
{

    auto sceneGraphWidget = dynamic_cast<SceneGraphWidget*>(_ui.mdiArea->activeSubWindow()->widget());
    if(sceneGraphWidget != nullptr)
    {
        auto newName = QInputDialog::getText(this, "Clone Scenario", "New Scenario Name : ", QLineEdit::Normal, QString("%1 - Clone").arg(sceneGraphWidget->getSceneGraph()->getModelGraph()->getName()));
        if(!newName.isEmpty())
        {
            cloneScenario(sceneGraphWidget->getSceneGraph(), newName);
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->modifiers() & Qt::Modifier::SHIFT)
	{
		auto now = std::chrono::steady_clock::now();
		if (!_dblClinkInitiated)
		{
			_dblClinkInitiated = true;
		}
		else
		{
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - _timeStamp);
			if (duration.count() < 300) // threshold
			{
				showNodeFilteringWidget();
				_dblClinkInitiated = false;
			}
		}
		_timeStamp = now;
	}
	event->accept();
}

QJSValue MainWindow::savetoJSValue(model::PersistenceHandler* handler) const
{
	auto sceneGraphWidgets = findChildren<SceneGraphWidget*>();
	auto uiValue = handler->createJsValue();
	auto scenesValue = handler->createJsValue();

    std::for_each(sceneGraphWidgets.begin(), sceneGraphWidgets.end(), [=, &scenesValue](SceneGraphWidget* sgw)
                  {
                      auto sceneGraph = sgw->getSceneGraph();
                      scenesValue.setProperty(sceneGraph->getModelGraph()->getIdentifier(), sgw->saveToJSValue(handler));
                  });

	uiValue.setProperty("scenes", scenesValue);
	return uiValue;
}

bool MainWindow::loadFromJSValue(const QJSValue& v)
{
    //auto sceneGraphWidgets = findChildren<SceneGraphWidget*>();

	auto scenesValue = v.property("scenes");
	QJSValueIterator it(scenesValue);
	while (it.hasNext())
	{
		it.next();
		auto sceneId = it.name();
		auto sceneValue = it.value();
		auto sgw = findChild<SceneGraphWidget*>(sceneId);
		if (sgw != nullptr)
		{
			sgw->loadFromJSValue(sceneValue);
		}
	}

	return true;
}

void MainWindow::showNodeFilteringWidget()
{
	//QMessageBox::information(this, "test", "test");
}

void MainWindow::openLastProject()
{
	auto recentProjects = _settingsManager->enumRecentProjects();
	if (!recentProjects.isEmpty())
	{
		openProject(recentProjects.first());
	}
}

void MainWindow::onOpenProject()
{
	auto fileName = QFileDialog::getOpenFileName(this, "Open project", _settingsManager->getLastOpenedLocation(), "RQFL Project (*.rqfl)");
	openProject(fileName);
}

void MainWindow::updateRecentProjectsList()
{
	auto recentProjects = _settingsManager->enumRecentProjects();
    //auto menu = _openProject->menu();
    auto menu = _recentProjectsMenu;
	auto actions = menu->actions();

	for (auto action : actions)
	{
		menu->removeAction(action);
		delete action;
	}

	// add an open project action
    for (const auto& prj : recentProjects)
    {        
        auto action = menu->addAction(QIcon(":/ui/network"), prj);
        connect(action, &QAction::triggered, this, [=]()
			{
				openProject(prj);
            });
	}

	if (recentProjects.size() > 0)
	{
		menu->addSeparator();
		auto action = menu->addAction("Clear recent projects list");
		auto font = action->font();
		font.setBold(true);
		action->setFont(font);
		action->setIcon(QIcon(":/ui/broom"));
        connect(action, &QAction::triggered, this, [=]()
			{
				if (QMessageBox::warning(this,
					"Warning",
					"Clear recent projects list",
					QMessageBox::Yes, QMessageBox::Cancel) == QMessageBox::Yes)
				{
					_settingsManager->clearRecentProjects();
					updateRecentProjectsList();
				}
            });
	}
	else
	{
		auto action = menu->addAction("Empty list");
		action->setEnabled(false);
	}
}

int MainWindow::onCloseProject()
{
	int button = -999;
	if (_project != nullptr)
	{
		button = QMessageBox::information(this, "Save", "Save the project before closing it?",
			QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
		switch (button)
		{
		case QMessageBox::StandardButton::Yes:
			onSaveProject();
			break;
		case QMessageBox::No:
			break;
		case QMessageBox::Cancel:
			return -1;
		}

        for(auto graph : _project->getGraphs())
        {
            graph->cancel();
        }
	}

	setWindowTitle("RequestFlow");

    _project.reset();

	qDeleteAll(_ui.mdiArea->subWindowList());
	_ui.mdiArea->closeAllSubWindows();

    _environmentsWidget->setProject(nullptr);
    _environmentsWidget->update();

    setProject(nullptr);
	return button;
}

void MainWindow::onSaveProject()
{
	if (_project != nullptr)
	{
		QString path = _project->getPath();
		if (path.isEmpty())
		{
			path = QFileDialog::getSaveFileName(this, "Save project", "", "RQFL Project (*.rqfl)");
			if (path.isEmpty()) return;
		}
		QFile fp(path);
		if (fp.open(QIODevice::WriteOnly))
		{
			model::PersistenceHandler handler;
			_project->setPath(path);
			auto projectValue = _project->saveToJSValue(&handler);
			auto uiValue = savetoJSValue(&handler);
			projectValue.setProperty("ui", uiValue);

			QTextStream out(&fp);
			out << handler.evaluate(projectValue);
			fp.close();
        }
    }
}

void MainWindow::onSaveProjectAs()
{
    if (_project != nullptr)
    {
        QString path = QFileDialog::getSaveFileName(this, "Save project As ...", _project->getPath(), "RQFL Project (*.rqfl)");
        if (path.isEmpty()) return;

        QFile fp(path);
        if (fp.open(QIODevice::WriteOnly))
        {
            model::PersistenceHandler handler;
            _project->setPath(path);
            auto projectValue = _project->saveToJSValue(&handler);
            auto uiValue = savetoJSValue(&handler);
            projectValue.setProperty("ui", uiValue);

            QTextStream out(&fp);
            out << handler.evaluate(projectValue);
            fp.close();
        }
    }
}

void MainWindow::onImportSwagger()
{
	if (_project)
	{
		auto fileName = QFileDialog::getOpenFileName(this, "Import swagger file", "", "JSON (*.json)");

		if (!fileName.isEmpty())
		{
			auto document = new model::Document(_project.get());
			if (!document->importFromSwagger(fileName))
			{
				delete document;
			}
			else
			{
				_ui.inventoryWidget->setProject(_project.get());
			}
		}
	}
}

void MainWindow::onSubWindowActivated(QMdiSubWindow* subWindow)
{
}

void MainWindow::onNewProject()
{
    if(onCloseProject() != -1)
    {
        setProject(new model::Project(this));
    }
}

void MainWindow::onSceneDeleted(QString identifier)
{
	if (_subwindowsMap.contains(identifier))
	{
		delete _subwindowsMap.take(identifier);
	}
}

void MainWindow::onActivateScene(model::Graph* scene)
{
	if (_subwindowsMap.contains(scene->getIdentifier()))
	{
		_subwindowsMap[scene->getIdentifier()]->showMaximized();
	}
}

#include <view/Edge.h>
#include <QActionGroup>

void MainWindow::setupMenuBar()
{
    // File menu
    {
        _fileMenu = menuBar()->addMenu("File");
        _newProjectAction = _fileMenu->addAction(QIcon(":/ui/new_file"), "New...", [=](){onNewProject();}, QKeySequence("Ctrl+N"));
        _openProjectAction = _fileMenu->addAction(QIcon(":/ui/open_file"), "Open...", [=](){onOpenProject();}, QKeySequence("Ctrl+O"));
        _openProjectAction->setMenu(new QMenu);

        _recentProjectsMenu = _openProjectAction->menu();
        updateRecentProjectsList();

        _saveProjectAction = _fileMenu->addAction(QIcon(":/ui/save_file"), "Save", [=](){onSaveProject();}, QKeySequence("Ctrl+S"));
        _saveProjectAsAction =  _fileMenu->addAction(QIcon(), "Save As...", [=](){onSaveProjectAs();}, QKeySequence("Ctrl+Shift+S"));
        _fileMenu->addSeparator();
        _settingsAction = _fileMenu->addAction(QIcon(), "Settings...", [=](){});
        _settingsAction->setEnabled(false); // TODO: implement a settings dialog
        _fileMenu->addSeparator();
        _closeProjectAction = _fileMenu->addAction(QIcon(":/ui/close_file"), "Close Project", [=](){ onCloseProject(); });
        _fileMenu->addSeparator();
        _quitProjectAction = _fileMenu->addAction(QIcon(":/ui/close"), "Quit", [=](){close();}, QKeySequence("Ctrl+Q"));

        _saveProjectAction->setEnabled(false);
        _saveProjectAsAction->setEnabled(false);
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
                                                                    layout->setMargin(0);
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
