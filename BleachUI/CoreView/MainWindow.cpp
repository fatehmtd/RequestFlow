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

class BackgroundPaintFilter : public QObject
{
protected:
	QPixmap _pixmap;
	QColor _bgColor;
public:
	BackgroundPaintFilter(QObject* parent) : QObject(parent), _bgColor("#A0A0A0")
	{
		_pixmap = QPixmap(":/ui/requestflow_bg");
	}

	virtual ~BackgroundPaintFilter()
	{
	}

	virtual bool eventFilter(QObject* obj, QEvent* event) override
	{
		if (event->type() != QEvent::Paint) return QObject::eventFilter(obj, event);

		auto widget = dynamic_cast<QWidget*>(obj);
		auto xOffset = 0, yOffset = 0;
		auto width = widget->width();
		auto height = widget->height();

		//const int maxHeight = _pixmap.width(), maxWidth = _pixmap.height();
		//const int minHeight = height / 9, minWidth = maxWidth / 9;
		//const float aspect = (float)maxWidth / 800.0f;

		QPainter painter;
		painter.begin(widget);
		painter.fillRect(0, 0, width, height, _bgColor); // fill the background with a color
		//painter.drawPixmap(width - (_pixmap.width() + xOffset), height - (_pixmap.height() + yOffset), _pixmap.width(), _pixmap.height(), _pixmap);
		painter.drawPixmap(width / 2 - _pixmap.width() / 2, height / 2 - _pixmap.height() / 2, _pixmap.width(), _pixmap.height(), _pixmap);
		painter.end();

		return true;
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
		event->accept();
	else
		event->ignore();
}

void MainWindow::setupUi()
{
	_ui.setupUi(this);
	setWindowIcon(QIcon(":/ui/network"));
	//setMinimumSize(1280, 800);
	setupRibbonBar();
	setupSceneGraph();
	setupEnvironmentsWidget();

	// enable the background image
	_ui.mdiArea->viewport()->installEventFilter(new BackgroundPaintFilter(this));
}

#include <QFile>
#include <QTextStream>

#include <model/Document.h>

void MainWindow::setupRibbonBar()
{
	_ui.dockWidget->setTitleBarWidget(new QWidget());
	_ui.dockWidget->setFixedHeight(122);

	_toolbar = new ActionToolBar(_ui.tabWidget);

	//_ui.tabWidget->setCornerWidget(new QPushButton("Test"), Qt::Corner::TopLeftCorner);

	auto page = _toolbar->addPage("Project", QIcon(":/ui/project"));

	auto projectGroup = page->addGroup("Project");

	_newProject = projectGroup->addActionItem("New", [=]() { onNewProject(); }, QIcon(":/ui/new_file"));
	_openProject = projectGroup->addActionItem("Open", [=]() { onOpenProject(); }, QIcon(":/ui/open_file"));
	_openProject->setPopupMode(QToolButton::ToolButtonPopupMode::MenuButtonPopup);
	_openProject->setMenu(new QMenu());

	updateRecentProjectsList();

	_saveProject = projectGroup->addActionItem("Save", [=]() { onSaveProject(); }, QIcon(":/ui/save_file"));
	_closeProject = projectGroup->addActionItem("Close", [=]() { onCloseProject(); }, QIcon(":/ui/close_file"));

	_swaggerImport = projectGroup->addActionItem("Import", [=]()
		{
			onImportSwagger();
		}, QIcon(":/ui/swagger"));

	_swaggerImport->setEnabled(false);

	_saveProject->setEnabled(false);
	_closeProject->setEnabled(false);

	_scenariosGroup = page->addGroup("Scenarios");
	_scenariosGroup->setEnabled(false);

	//_scenariosGroup->addActionItem("Start/Stop", [=]() {}, QIcon(":/BleachUI/play"));
	_scenariosGroup->addActionItem("New", [=]()
		{
			auto name = QInputDialog::getText(this, "Create Scenario", "Name :");
			if (!name.isEmpty())
			{
				createScenario(name);
			}
		}, QIcon(":/ui/test_case"));
	_scenariosGroup->addActionItem("Export", [=]() {}, QIcon(":/ui/export"));
	_scenariosGroup->addActionItem("Import", [=]() {}, QIcon(":/ui/import"));
}

void MainWindow::setupEnvironmentsWidget()
{
	//_ui.environmentsWidget->setProject(_sceneGraphWidget->getSceneGraph()->getModelGraph());
	_ui.environmentsWidget->setEnabled(false);
	connect(_ui.environmentsWidget, &EnvironmentsWidget::currentEnvironmentChanged, this, &MainWindow::onCurrentEnvironmentChanged);
}

#include "view/Node.h"
#include <QPropertyAnimation>

void MainWindow::setupSceneGraph()
{
	_ui.mdiArea->setTabsClosable(false);
	connect(_ui.mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::onSubWindowActivated);
	connect(_ui.scenariosWidget, &ScenariosWidget::sceneDeleted, this, &MainWindow::onSceneDeleted);
	connect(_ui.scenariosWidget, &ScenariosWidget::currentSceneChanged, this, &MainWindow::onActivateScene);

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

	_ui.environmentsWidget->setEnabled(projectAvailable);
	_ui.inventoryWidget->setEnabled(projectAvailable);
	_ui.scenariosWidget->setEnabled(projectAvailable);

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

		_ui.scenariosWidget->setProject(_project.get());
	}

	_closeProject->setEnabled(projectAvailable);
	_saveProject->setEnabled(projectAvailable);
	_scenariosGroup->setEnabled(projectAvailable);
	_swaggerImport->setEnabled(projectAvailable);
	_ui.inventoryWidget->setProject(project);
	_ui.logMessagesWidget->setProject(project);
	_ui.environmentsWidget->setProject(project);
}

void MainWindow::createScenario(QString name)
{
	auto graph = new model::Graph(_project.get());
	graph->setName(name);
	openScenario(new view::SceneGraph(graph));
	_ui.scenariosWidget->updateScenariosList();
	_ui.logMessagesWidget->addMessageLogger(graph->getLogger());
}

void MainWindow::openScenario(view::SceneGraph* sceneGraph)
{
	auto sceneGraphWidget = new SceneGraphWidget(this, sceneGraph);

	sceneGraphWidget->setAttribute(Qt::WA_DeleteOnClose, false);
	sceneGraphWidget->setWindowFlag(Qt::WindowType::WindowCloseButtonHint, false);

	sceneGraphWidget->setWindowIcon(QIcon(":/ui/test_case"));
	auto window = _ui.mdiArea->addSubWindow(sceneGraphWidget);

	_subwindowsMap.insert(sceneGraph->getModelGraph()->getIdentifier(), window);

	window->setAttribute(Qt::WA_DeleteOnClose, false);
	window->setWindowFlag(Qt::WindowType::WindowCloseButtonHint, false);
	window->setWindowIcon(QIcon(":/ui/test_case"));
	window->setWindowTitle(sceneGraph->getModelGraph()->getName());

	connect(sceneGraph->getModelGraph(), &model::IdentifiableEntity::nameChanged, window, &QMdiSubWindow::setWindowTitle);
	window->showMaximized();
}

void MainWindow::cloneScenario(view::SceneGraph* sceneGraph, QString newName)
{
}

void MainWindow::deleteScenario(view::SceneGraph* sceneGraph)
{
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

#include "../view/Node.h"
#include "../view/SceneGraph.h"

QJSValue MainWindow::savetoJSValue(model::PersistenceHandler* handler) const
{
	auto sceneGraphWidgets = findChildren<SceneGraphWidget*>();
	auto uiValue = handler->createJsValue();
	auto scenesValue = handler->createJsValue();
	for (auto sgw : sceneGraphWidgets)
	{
		auto sceneGraph = sgw->getSceneGraph();
		scenesValue.setProperty(sceneGraph->getModelGraph()->getIdentifier(), sgw->saveToJSValue(handler));
	}
	uiValue.setProperty("scenes", scenesValue);
	return uiValue;
}

#include <QJSValueIterator>

bool MainWindow::loadFromJSValue(const QJSValue& v)
{
	auto sceneGraphWidgets = findChildren<SceneGraphWidget*>();

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

void MainWindow::onOpenProject()
{
	auto fileName = QFileDialog::getOpenFileName(this, "Open project", _settingsManager->getLastOpenedLocation(), "RQFL Project (*.rqfl)");
	openProject(fileName);
}

void MainWindow::updateRecentProjectsList()
{
	auto recentProjects = _settingsManager->enumRecentProjects();
	auto menu = _openProject->menu();
	auto actions = menu->actions();

	for (auto action : actions)
	{
		menu->removeAction(action);
		delete action;
	}

	// add an open project action
	for (auto prj : recentProjects)
	{
		auto action = menu->addAction(QIcon(":/ui/network"), prj);
		connect(action, &QAction::triggered, [=]()
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
		connect(action, &QAction::triggered, [=]()
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

#include <QMessageBox>

int MainWindow::onCloseProject()
{
	if (_project != nullptr)
	{
		int button = QMessageBox::information(this, "Save ?", "Save the project before closing it?",
			QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
		switch (button)
		{
		case QMessageBox::StandardButton::Yes:
			onSaveProject();
			return 1;
		case QMessageBox::No:
			break;
		case QMessageBox::Cancel:
			return -1;
		}
	}
	_project.reset();
	_scenariosGroup->setEnabled(false);

	qDeleteAll(_ui.mdiArea->subWindowList());
	_ui.mdiArea->closeAllSubWindows();

	_ui.environmentsWidget->setProject(nullptr);
	_ui.scenariosWidget->setProject(nullptr);
	_ui.environmentsWidget->update();
	_ui.scenariosWidget->update();

	setProject(nullptr);
	return 0;
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

void MainWindow::onCurrentEnvironmentChanged(model::Environment* environment)
{
	_activeEnvironment = environment;
	if (_activeGraph != nullptr)
	{
		_activeGraph->setActiveEnvironment(_activeEnvironment);
	}
}

void MainWindow::onSubWindowActivated(QMdiSubWindow* subWindow)
{
	_activeGraph = nullptr;

	if (subWindow != nullptr)
	{
		auto sceneGraphWidget = dynamic_cast<SceneGraphWidget*>(subWindow->widget());
		if (sceneGraphWidget != nullptr)
		{
			_activeGraph = sceneGraphWidget->getSceneGraph()->getModelGraph();
			_activeGraph->setActiveEnvironment(_ui.environmentsWidget->getActiveEnvironment());
		}
	}
}

void MainWindow::onNewProject()
{
	onCloseProject();
	setProject(new model::Project(this));
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
