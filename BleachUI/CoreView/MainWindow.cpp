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
	setupUi();
}

void MainWindow::setupUi()
{
	_ui.setupUi(this);
	setWindowIcon(QIcon(":/BleachUI/graph"));
	//setMinimumSize(1280, 800);
	setupRibbonBar();
	setupSceneGraph();
	setupEnvironmentsWidget();

	// enable the background image
	_ui.mdiArea->viewport()->installEventFilter(new BackgroundPaintFilter(this));
}

#include <QFile>
#include <QTextStream>

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
	_saveProject = projectGroup->addActionItem("Save", [=]() { onSaveProject(); }, QIcon(":/ui/save_file"));
	_closeProject = projectGroup->addActionItem("Close", [=]() { onCloseProject(); }, QIcon(":/ui/close_file"));
	
	projectGroup->addActionItem("Import", [=]() {}, QIcon(":/ui/swagger"));

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

void MainWindow::setupSceneGraph()
{
	_ui.mdiArea->setTabsClosable(false);
	connect(_ui.mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::onSubWindowActivated);
	connect(_ui.scenariosWidget, &ScenariosWidget::sceneDeleted, this, &MainWindow::onSceneDeleted);
	connect(_ui.scenariosWidget, &ScenariosWidget::currentSceneChanged, this, &MainWindow::onActivateScene);
}

void MainWindow::setProject(model::Project* project)
{
	bool projectAvailable = project != nullptr;

	_closeProject->setEnabled(projectAvailable);
	_saveProject->setEnabled(projectAvailable);
	_scenariosGroup->setEnabled(projectAvailable);

	if (projectAvailable)
	{
		_ui.environmentsWidget->setEnabled(true);

		_project.reset(project);

		if (project->getEnvironments().isEmpty())
		{
			auto environment = new model::Environment(project);
			environment->setName(QString("Default environment"));
			environment->getEntries().insert("baseUrl", "http://localhost");
		}

		_ui.environmentsWidget->setProject(project);

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
	else
	{

	}
}

void MainWindow::createScenario(QString name)
{
	auto graph = new model::Graph(_project.get());
	graph->setName(name);
	openScenario(new view::SceneGraph(graph));
	_ui.scenariosWidget->updateScenariosList();
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

void MainWindow::deleteScenario(view::SceneGraph* sceneGraph)
{
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

void MainWindow::onOpenProject()
{
	auto fileName = QFileDialog::getOpenFileName(this, "Open project", "", "RQFL Project (*.rqfl)");

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
			}
		}
	}
}

void MainWindow::onCloseProject()
{
	_project.reset();
	_scenariosGroup->setEnabled(false);

	qDeleteAll(_ui.mdiArea->subWindowList());
	_ui.mdiArea->closeAllSubWindows();

	_ui.environmentsWidget->setProject(nullptr);
	_ui.scenariosWidget->setProject(nullptr);
	_ui.environmentsWidget->update();
	_ui.scenariosWidget->update();

	setProject(nullptr);
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
		//qDebug() << sceneGraphWidget;
		if (sceneGraphWidget != nullptr)
		{
			//onActivateScene(sceneGraphWidget->getSceneGraph()->getModelGraph());
			_activeGraph = sceneGraphWidget->getSceneGraph()->getModelGraph();
			_activeGraph->setActiveEnvironment(_ui.environmentsWidget->getActiveEnvironment());
		}
	}
}

void MainWindow::onNewProject()
{
	onCloseProject();
	setProject(new model::Project(this));
	/*
	auto fileName = QFileDialog::getSaveFileName(this, "Create project", "", "RQFL Project (*.rqfl)");

	if (!fileName.isEmpty())
	{

	}*/
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
