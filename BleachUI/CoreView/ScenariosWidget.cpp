#include "ScenariosWidget.h"

ScenariosWidget::ScenariosWidget(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
}

ScenariosWidget::~ScenariosWidget()
{
}

void ScenariosWidget::setProject(model::Project* project)
{
	_project = std::make_unique<model::Project>(project);
}

model::Project* const ScenariosWidget::getProject() const
{
	return _project.get();
}

void ScenariosWidget::setCurrentSceneGraph(view::SceneGraph* sceneGraph)
{
	_currentSceneGraph = sceneGraph;
	//TODO: select the correct item in the list
}

view::SceneGraph* const ScenariosWidget::getCurrentSceneGraph() const
{
	return _currentSceneGraph;
}

void ScenariosWidget::fillScenariosList()
{

}
