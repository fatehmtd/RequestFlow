#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "./view/SceneGraphWidget.h"

#include "ActionToolBar.h"
#include "ActionPage.h"
#include "ActionGroup.h"
#include "ActionItem.h"

#include <model/Project.h>
#include <model/PersistenceHandler.h>
#include <model/PersistableEntity.h>
#include <QMap>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

public slots:
    void onSceneDeleted(QString identifier);
    void onActivateScene(model::Graph* scene);    

private slots:
    void onNewProject();
    void onOpenProject();
    void onCloseProject();
    void onSaveProject();
    void onCurrentEnvironmentChanged(model::Environment* environment);
    void onSubWindowActivated(QMdiSubWindow* subWindow);

private:
    void setupUi();
    void setupRibbonBar();
    void setupEnvironmentsWidget();
    void setupSceneGraph();

    void setProject(model::Project* project);

    void createScenario(QString name);
    void openScenario(view::SceneGraph* sceneGraph);
    void deleteScenario(view::SceneGraph* sceneGraph);

    QJSValue savetoJSValue(model::PersistenceHandler* handler) const;
    bool loadFromJSValue(const QJSValue& v);

private:
    Ui::MainWindowClass _ui;

    ActionToolBar* _toolbar = nullptr;
    QToolButton* _newProject, * _openProject, * _closeProject, * _saveProject;
    ActionGroup* _scenariosGroup = nullptr;

    model::Environment* _activeEnvironment = nullptr;
    model::Graph* _activeGraph = nullptr;

    std::unique_ptr<model::Project> _project;
    QMap<QString, QMdiSubWindow*> _subwindowsMap;
};
