#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "./view/SceneGraphWidget.h"

#include "ActionToolBar.h"
#include "ActionPage.h"
#include "ActionGroup.h"
#include "ActionItem.h"
#include "SettingsManager.h"

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
    void onImportSwagger();
    void onCurrentEnvironmentChanged(model::Environment* environment);
    void onSubWindowActivated(QMdiSubWindow* subWindow);

private:
    void closeEvent(QCloseEvent* event) override;
    void setupUi();
    void setupRibbonBar();
    void setupEnvironmentsWidget();
    void setupSceneGraph();
    void openProject(const QString& path);
    void setProject(model::Project* project);

    void createScenario(QString name);
    void openScenario(view::SceneGraph* sceneGraph);
    void cloneScenario(view::SceneGraph* sceneGraph, QString newName);
    void deleteScenario(view::SceneGraph* sceneGraph);

    void updateRecentProjectsList();

    QJSValue savetoJSValue(model::PersistenceHandler* handler) const;
    bool loadFromJSValue(const QJSValue& v);

private:
    Ui::MainWindowClass _ui;

    ActionToolBar* _toolbar = nullptr;
    QToolButton* _newProject, * _openProject, * _closeProject, * _saveProject, *_swaggerImport;
    ActionGroup* _scenariosGroup = nullptr;

    model::Environment* _activeEnvironment = nullptr;
    model::Graph* _activeGraph = nullptr;

    std::unique_ptr<model::Project> _project;
    QMap<QString, QMdiSubWindow*> _subwindowsMap;
    view::SettingsManager* _settingsManager = nullptr;
};
