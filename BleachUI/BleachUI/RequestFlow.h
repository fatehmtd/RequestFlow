#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RequestFlow.h"
#include "./view/SceneGraphWidget.h"

#include "ActionToolBar.h"
#include "ActionPage.h"
#include "ActionGroup.h"
#include "ActionItem.h"

#include <model/Project.h>

class RequestFlow : public QMainWindow
{
    Q_OBJECT
public:
    RequestFlow(QWidget *parent = Q_NULLPTR);

private:
    void setupUi();
    void setupRibbonBar();
    void setupEnvironmentsWidget();
    void setupSceneGraph();

    void setProject(model::Project* project);

private slots:
    void onNewProject();
    void onOpenProject();
    void onCloseProject();

private:
    Ui::RequestFlowClass _ui;
    SceneGraphWidget* _sceneGraphWidget = nullptr;
    ActionToolBar* _toolbar = nullptr;
    QToolButton* _newProject, * _openProject, * _closeProject, * _saveProject;

    std::unique_ptr<model::Project> _project;
};
