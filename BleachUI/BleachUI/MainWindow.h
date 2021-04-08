#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "./view/SceneGraphWidget.h"

#include "ActionToolBar.h"
#include "ActionPage.h"
#include "ActionGroup.h"
#include "ActionItem.h"

#include <model/Project.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = Q_NULLPTR);

private:
    void setupUi();
    void setupRibbonBar();
    void setupEnvironmentsWidget();
    void setupSceneGraph();

    void setProject(model::Project* project);
    
    void createScenario(QString name);
    void openScenario(view::SceneGraph* sceneGraph);
    void deleteScenario(view::SceneGraph* sceneGraph);

private slots:
    void onNewProject();
    void onOpenProject();
    void onCloseProject();

private:
    Ui::MainWindowClass _ui;

    ActionToolBar* _toolbar = nullptr;
    QToolButton* _newProject, * _openProject, * _closeProject, * _saveProject;
    ActionGroup* _scenariosGroup = nullptr;

    std::unique_ptr<model::Project> _project;
};
