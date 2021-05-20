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
    void onCurrentEnvironmentChanged(model::Environment* environment);
    void onSubWindowActivated(QMdiSubWindow* subWindow);

private:
    void closeEvent(QCloseEvent* event) override;
    void setupUi();
    void setupMenuBar();
    void setupRibbonBar();
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
    void openScenario(view::SceneGraph* sceneGraph);
    void cloneScenario(view::SceneGraph* sceneGraph, QString newName);
    void deleteScenario(view::SceneGraph* sceneGraph);

    void keyPressEvent(QKeyEvent* event) override;

    void updateRecentProjectsList();

    QJSValue savetoJSValue(model::PersistenceHandler* handler) const;
    bool loadFromJSValue(const QJSValue& v);

    void showNodeFilteringWidget();

    void openLastProject();
private:
    Ui::MainWindowClass _ui;

    //////////////////////////////////////////////
    /// \brief Menus
    //////////////////////////////////////////////

    // File
    QMenu* _fileMenu = nullptr;
    QAction* _newProjectAction = nullptr;
    QAction* _openProjectAction= nullptr;
    QAction* _saveProjectAction= nullptr;
    QAction* _saveProjectAsAction= nullptr;
    QAction* _closeProjectAction = nullptr;
    QAction* _quitProjectAction = nullptr;
    QAction* _settingsAction = nullptr;
    QMenu* _recentProjectsMenu = nullptr;

    // Scenarios
    QMenu* _scenariosMenu = nullptr;
    QAction* _createScenarioAction = nullptr;
    QAction* _cloneScenarioAction = nullptr;

    // Environments
    QMenu* _environmentsMenu = nullptr;
    QAction* _environmentConfigAction = nullptr;
    QAction* _importSwaggerAction = nullptr;

    // View
    QMenu* _viewMenu = nullptr;
    QAction* _switchThemeAction = nullptr;
    QAction* _centerOnAction = nullptr;

    // About
    QMenu* _helpMenu = nullptr;

    //*/
    //////////////////////////////////////////////

    bool _dblClinkInitiated = false;
    std::chrono::steady_clock::time_point _timeStamp;

    model::Environment* _activeEnvironment = nullptr;
    model::Graph* _activeGraph = nullptr;

    std::unique_ptr<model::Project> _project;
    QMap<QString, QMdiSubWindow*> _subwindowsMap;
    view::SettingsManager* _settingsManager = nullptr;

    //////////////////////////////////////////////////

};
