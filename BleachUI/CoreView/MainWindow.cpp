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
	_saveProject = projectGroup->addActionItem("Save", [=]() {}, QIcon(":/ui/save_file"));
	_closeProject = projectGroup->addActionItem("Close", [=]() { onCloseProject(); }, QIcon(":/ui/close_file"));

	_saveProject->setEnabled(false);
	_closeProject->setEnabled(false);

	_scenariosGroup = page->addGroup("Scenarios");
	_scenariosGroup->setEnabled(false);

	//casesGroup->addActionItem("Start/Stop", [=]() {}, QIcon(":/BleachUI/play"));
	_scenariosGroup->addActionItem("New", [=]()
		{
			QString name = QString("Scenario %1").arg(rand() % 999999);
			createScenario(name);
		}, QIcon(":/ui/test_case"));
	_scenariosGroup->addActionItem("Export", [=]() {}, QIcon(":/ui/export"));
	_scenariosGroup->addActionItem("Import", [=]() {}, QIcon(":/ui/import"));
}

void MainWindow::setupEnvironmentsWidget()
{
	//_ui.environmentsWidget->setProject(_sceneGraphWidget->getSceneGraph()->getModelGraph());
	_ui.environmentsWidget->setEnabled(false);
}

void MainWindow::setupSceneGraph()
{
	_ui.mdiArea->setTabsClosable(false);
}

void MainWindow::setProject(model::Project* project)
{
	_ui.environmentsWidget->setEnabled(true);

	_project = std::make_unique<model::Project>(project);
	_scenariosGroup->setEnabled(true);

	if (_project->getEnvironments().isEmpty())
	{
		auto environment = new model::Environment(_project.get());
		environment->setName(QString("Default environment"));
	}

	_ui.environmentsWidget->setProject(_project.get());
	_ui.scenariosWidget->setProject(_project.get());
}

void MainWindow::createScenario(QString name)
{
	auto graph = new model::Graph(_project.get());
	graph->setName(QString("Scenario %1").arg(rand() % 10000));
	openScenario(new view::SceneGraph(graph));
}

void MainWindow::openScenario(view::SceneGraph* sceneGraph)
{
	//auto sceneGraph = new view::SceneGraph(new model::Graph(_project.get()));
	auto sceneGraphWidget = new SceneGraphWidget(this, sceneGraph);
	sceneGraphWidget->setWindowIcon(QIcon(":/ui/test_case"));
	auto window = _ui.mdiArea->addSubWindow(sceneGraphWidget);
	window->setWindowFlag(Qt::WindowType::WindowCloseButtonHint, false);
	window->setWindowIcon(QIcon(":/ui/test_case"));
	window->setWindowTitle(sceneGraph->getModelGraph()->getName());
	window->showMaximized();
}

void MainWindow::deleteScenario(view::SceneGraph* sceneGraph)
{
}

void MainWindow::onOpenProject()
{
	auto fileName = QFileDialog::getOpenFileName(this, "Open project", "", "RQFL Project (*.rqfl)");

	if (!fileName.isEmpty())
	{

	}
}

void MainWindow::onCloseProject()
{
	_project.reset();
	_scenariosGroup->setEnabled(false);
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
