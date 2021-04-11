#pragma once

#include <QWidget>
#include <QAbstractItemModel>
#include "ui_ScenariosWidget.h"

#include "SceneGraph.h"
#include <model/Project.h>
#include <model/Graph.h>

class ScenariosModel;

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

private slots:
	void fillScenariosList();
	void onContextMenuRequested(const QPoint& p);

signals:
	void currentScenarioChanged(view::SceneGraph*);

private:
	Ui::ScenariosWidget _ui;
	model::Project* _project = nullptr;
	view::SceneGraph* _currentSceneGraph = nullptr;
	ScenariosModel* _scenariosModel = nullptr;
};
