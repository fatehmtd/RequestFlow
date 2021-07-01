#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <view/SceneGraphWidget.h>

#include "ActionToolBar.h"
#include "ActionPage.h"
#include "ActionGroup.h"
#include "ActionItem.h"
#include "SettingsManager.h"

#include <model/Project.h>
#include <model/PersistenceHandler.h>
#include <model/PersistableEntity.h>
#include <QMap>
#include <chrono>

#include "EnvironmentsWidget.h"

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
    int onCloseProject();
    void onSaveProject();
    void onSaveProjectAs();
    void onImportSwagger();

private:
    void closeEvent(QCloseEvent* event) override;
    void setupUi();
    void setupMenuBar();
    void setupEnvironmentsWidget();
    void setupSceneGraph();
    void openProject(const QString& path);
    void setProject(model::Project* project);

    // about menu
    void onAbout();
    void onContactSupport();
    void onWebsite();
    void onActivateLicense();
    void onTwitter();

    void createScenario(QString name);
    SceneGraphWidget* openScenario(view::SceneGraph* sceneGraph);
    void cloneScenario(view::SceneGraph* sceneGraph, QString newName);
    void deleteScenario(view::SceneGraph* sceneGraph);
    void deleteActiveScenario();
    void cloneActiveScenario();

    void updateRecentProjectsList();

    QJSValue savetoJSValue(model::PersistenceHandler* handler) const;
    bool loadFromJSValue(const QJSValue& v);

    void openLastProject();

    void setMiniMapLocation(int location);
    void setMiniMapStatus(bool status);

private:
    SceneGraphWidget *getActiveSceneGraphWidget() const;

private:
    Ui::MainWindowClass _ui;

    //////////////////////////////////////////////
    /// \brief Menus
    //////////////////////////////////////////////

    // File
    QMenu* _fileMenu = nullptr;
    QAction* _newProjectAction = nullptr;
    QAction* _openProjectAction= nullptr;
    QAction* _recentProjectsAction= nullptr;
    QAction* _saveProjectAction= nullptr;
    QAction* _saveProjectAsAction= nullptr;
    QAction* _closeProjectAction = nullptr;
    QAction* _quitProjectAction = nullptr;
    QAction* _settingsAction = nullptr;
    QMenu* _recentProjectsMenu = nullptr;

    // Edit
    QMenu *_editMenu = nullptr;
    QAction *_undoAction = nullptr;
    QAction *_redoAction = nullptr;
    QAction *_findNodeAction = nullptr;

    // Scenarios
    QMenu* _scenariosMenu = nullptr;
    QAction* _createScenarioAction = nullptr;
    QAction* _cloneScenarioAction = nullptr;
    QAction* _deleteScenarioAction = nullptr;

    // Environments
    QMenu* _toolsMenu = nullptr;
    QAction* _environmentConfigAction = nullptr;
    QAction* _importSwaggerAction = nullptr;

    // View
    QMenu* _viewMenu = nullptr;
    QAction * _miniMapAction = nullptr;
    QMenu* _miniMapLocationMenu = nullptr;
    QAction* _switchThemeAction = nullptr;
    QAction* _centerOnAction = nullptr;

    // About
    QMenu* _helpMenu = nullptr;

    //////////////////////////////////////////////

    bool _dblClinkInitiated = false;
    std::chrono::steady_clock::time_point _timeStamp;

    std::unique_ptr<model::Project> _project;
    QMap<QString, QMdiSubWindow*> _subwindowsMap;
    view::SettingsManager* _settingsManager = nullptr;

    //EnvironmentsWidget* _environmentsWidget = nullptr;

    //////////////////////////////////////////////////

};
