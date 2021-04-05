#include "RequestFlow.h"
#include "./view/SceneGraph.h"
#include "./view/SceneGraphWidget.h"
#include <QDockWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QDebug>

RequestFlow::RequestFlow(QWidget* parent) : QMainWindow(parent)
{
	setupUi();
}

void RequestFlow::setupUi()
{
	_ui.setupUi(this);
	setWindowIcon(QIcon(":/BleachUI/graph"));
	//setMinimumSize(1280, 800);
	setupRibbonBar();
	setupSceneGraph();
	setupEnvironmentsWidget();
}

void RequestFlow::setupRibbonBar()
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

	auto casesGroup = page->addGroup("Test cases");
	casesGroup->setEnabled(false);

	//casesGroup->addActionItem("Start/Stop", [=]() {}, QIcon(":/BleachUI/play"));
	casesGroup->addActionItem("New", [=]()
		{
		}, QIcon(":/ui/test_case"));
	casesGroup->addActionItem("Export", [=]() {}, QIcon(":/ui/export"));
	casesGroup->addActionItem("Import", [=]() {}, QIcon(":/ui/import"));
}

void RequestFlow::setupEnvironmentsWidget()
{
	//_ui.environmentsWidget->setProject(_sceneGraphWidget->getSceneGraph()->getModelGraph());
	_ui.environmentsWidget->setEnabled(false);
}

void RequestFlow::setupSceneGraph()
{
	/*
	_sceneGraphWidget = new SceneGraphWidget(this);
	_sceneGraphWidget->setWindowIcon(QIcon(":/ui/test_case"));
	auto window = _ui.mdiArea->addSubWindow(_sceneGraphWidget);
	window->setWindowFlag(Qt::WindowType::WindowCloseButtonHint, false);
	window->setWindowIcon(QIcon(":/ui/test_case"));
	window->setWindowTitle("Test case");
	window->showMaximized();
	*/
	_ui.mdiArea->setTabsClosable(false);
}

void RequestFlow::setProject(model::Project* project)
{
	_project = std::make_unique<model::Project>(project);
}

void RequestFlow::onOpenProject()
{
	auto fileName = QFileDialog::getOpenFileName(this, "Open project", "", "RQFL Project (*.rqfl)");

	if (!fileName.isEmpty())
	{

	}
}

void RequestFlow::onCloseProject()
{
	_project.reset();
}

void RequestFlow::onNewProject()
{
	onCloseProject();
	setProject(new model::Project(this));
	auto fileName = QFileDialog::getSaveFileName(this, "Create project", "", "RQFL Project (*.rqfl)");

	if (!fileName.isEmpty())
	{

	}
}
