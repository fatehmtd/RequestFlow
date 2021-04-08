#pragma once

#include <QWidget>
#include <QAbstractItemModel>
#include "ui_ScenariosWidget.h"

#include "SceneGraph.h"
#include <model/Project.h>
#include <model/Graph.h>

class ScenariosWidget : public QWidget
{
	Q_OBJECT
public:
	ScenariosWidget(QWidget* parent = nullptr);
	~ScenariosWidget();

	void setProject(model::Project* project);
	model::Project* const getProject() const;

	void setCurrentSceneGraph(view::SceneGraph* sceneGraph);
	view::SceneGraph* const getCurrentSceneGraph() const;
private:
	void fillScenariosList();
private:
	Ui::ScenariosWidget ui;
	std::unique_ptr<model::Project> _project;
	view::SceneGraph* _currentSceneGraph = nullptr;
};
