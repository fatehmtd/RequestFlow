#include "MainWindow.h"
#include "./view/SceneGraph.h"
#include "./view/SceneGraphWidget.h"
#include <QDockWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QDebug>
#include <QObject>
#include <QPixmap>
#include <QPainter>
#include <QEvent>
#include <QWidget>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include "BackgroundPaintFilter.h"
#include <view/Node.h>
#include <view/SceneGraph.h>
#include <QFile>
#include <QTextStream>
#include <QJSValueIterator>
#include <model/Document.h>
#include "view/Node.h"
#include <QPropertyAnimation>
#include "AboutWidget.h"
#include <QDialog>
#include <QSizePolicy>
#include <QDesktopServices>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QDialog>


class CloseEventIgnoreEventFilter : public QObject
{
public:
    using QObject::QObject;

    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if(event->type() == QEvent::Type::Close)
        {
            event->ignore();
            return true;
        }
        return QObject::eventFilter(watched, event);
    }
};

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    _settingsManager = new view::SettingsManager(this);
    _settingsManager->applyProxySetting();
    setupUi();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (onCloseProject() != -1)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::setupUi()
{
    _ui.setupUi(this);

    preparePalettes();

    setWindowIcon(QIcon(":/ui/network"));
    //setMinimumSize(1280, 800);
    setupMenuBar();
    setupSceneGraph();
    setupEnvironmentsWidget();

    setTheme(_settingsManager->getTheme());

    // enable the background image
    _ui.mdiArea->viewport()->installEventFilter(new BackgroundPaintFilter(this));
    //openLastProject();

    setProject(nullptr);
}

void MainWindow::setupEnvironmentsWidget()
{
    //_environmentsWidget = new EnvironmentsWidget();
}

void MainWindow::setupSceneGraph()
{
    _ui.mdiArea->setTabsClosable(false);

    connect(_ui.logMessagesWidget, &LogMessagesWidget::senderSelected, this, [=](model::Node* node)
    {
        auto subWindowsList = _ui.mdiArea->subWindowList();
        std::for_each(subWindowsList.begin(), subWindowsList.end(), [=](QMdiSubWindow* subWindow) {
            auto sceneGraphWidget = dynamic_cast<SceneGraphWidget*>(subWindow->widget());
            if (sceneGraphWidget != nullptr)
            {
                if (sceneGraphWidget->getSceneGraph()->getModelGraph() == node->getGraph())
                {
                    auto nodeGr = sceneGraphWidget->getSceneGraph()->findbyModel(node);

                    _ui.mdiArea->setActiveSubWindow(subWindow);

                    sceneGraphWidget->setCenterAnimated(nodeGr);
                }
            }
        }
        );});
}

void MainWindow::openProject(const QString& fileName)
{
    if (!fileName.isEmpty())
    {
        QFile fp(fileName);
        if (fp.open(QIODevice::ReadOnly))
        {
            model::PersistenceHandler handler;
            QString contents(QString("(%1)").arg(QString(QJsonDocument::fromJson(fp.readAll()).toJson(QJsonDocument::JsonFormat::Compact))));

            auto projectValue = handler.evaluate(contents);

            if (!projectValue.isError())
            {
                onCloseProject();

                auto project = new model::Project(nullptr);
                project->setPath(fileName);
                project->loadFromJSValue(projectValue);

                setWindowTitle(QString("RequestFlow - %1").arg(fileName));

                setProject(project);

                loadFromJSValue(projectValue.property("ui"));

                _settingsManager->addRecentProject(fileName);
            }

            updateRecentProjectsList();
        }
    }
}

void MainWindow::setProject(model::Project* project)
{
    bool projectAvailable = project != nullptr;

    _ui.environmentsWidget->setEnabled(projectAvailable);
    _ui.inventoryWidget->setEnabled(projectAvailable);
    _ui.dockWidget_inventory->setVisible(projectAvailable);
    _ui.dockWidget_environments->setVisible(projectAvailable);
    _ui.dockWidget_logs->setVisible(projectAvailable);

    // fill default objects if the project is empty
    if (projectAvailable)
    {
        _project.reset(project);

        if (project->getEnvironments().isEmpty())
        {
            auto environment = new model::Environment(project);
            environment->setName(QString("Default environment"));
            environment->getEntries().insert("baseUrl", "http://localhost");
        }

        if (project->getGraphs().isEmpty())
        {
            auto graph = new model::Graph(_project.get());
            graph->setName("Default scenario");
            auto sceneGraph = new view::SceneGraph(graph);
            openScenario(sceneGraph);
        }
        else
        {
            for (auto modelGraph : _project->getGraphs())
            {
                auto sceneGraph = new view::SceneGraph(modelGraph);
                openScenario(sceneGraph);
            }
        }
    }

    // File
    _saveProjectAction->setEnabled(projectAvailable);
    _saveProjectAsAction->setEnabled(projectAvailable);
    _closeProjectAction->setEnabled(projectAvailable);

    // Edit
    _editMenu->setEnabled(projectAvailable);

    // View
    _viewMenu->setEnabled(projectAvailable);

    // Scenarios
    _scenariosMenu->setEnabled(projectAvailable);

    // Environments
    _toolsMenu->setEnabled(projectAvailable);

    _ui.inventoryWidget->setProject(project);
    _ui.logMessagesWidget->setProject(project);
    _ui.environmentsWidget->setProject(project);
}


void MainWindow::onAbout()
{
    auto dialog = new QDialog(this);
    dialog->setWindowFlag(Qt::WindowType::WindowCloseButtonHint, true);
    dialog->setWindowFlag(Qt::WindowType::WindowMinMaxButtonsHint, false);
    dialog->setWindowFlag(Qt::WindowType::WindowContextHelpButtonHint, false);
    dialog->setWindowFlag(Qt::WindowType::WindowTitleHint, false);
    //dialog->setSizePolicy(QSizePolicy::Policy::Fixed);
    auto widget = new AboutWidget(dialog);
    dialog->setFixedSize(widget->size());
    dialog->exec();
}

void MainWindow::onContactSupport()
{
    QDesktopServices::openUrl(QUrl("mailto:fateh@requestflow.dev?subject=Regarding RequestFlow"));
}

void MainWindow::onWebsite()
{
    QDesktopServices::openUrl(QUrl("http://www.requestflow.dev"));
}

void MainWindow::onActivateLicense()
{

}

void MainWindow::onTwitter()
{
    QDesktopServices::openUrl(QUrl("https://twitter.com/requestflow"));
}

void MainWindow::createScenario(QString name)
{
    auto graph = new model::Graph(_project.get());
    graph->setName(name);
    openScenario(new view::SceneGraph(graph));
    _ui.logMessagesWidget->addMessageLogger(graph->getLogger());
}

#include <model/Environment.h>

SceneGraphWidget* MainWindow::openScenario(view::SceneGraph* sceneGraph)
{
    auto sceneGraphWidget = new SceneGraphWidget(this, sceneGraph);

    sceneGraphWidget->setAttribute(Qt::WA_DeleteOnClose, false);
    sceneGraphWidget->setWindowFlag(Qt::WindowType::WindowCloseButtonHint, false);

    sceneGraph->setBackgroundType((view::SceneGraph::BackgroundType)_settingsManager->getBackgroundType());
    sceneGraph->setEdgeType((view::SceneGraph::EdgeType)_settingsManager->getEdgesStyle());

    sceneGraphWidget->setWindowIcon(QIcon(":/ui/test_case"));
    auto window = _ui.mdiArea->addSubWindow(sceneGraphWidget);

    // MiniMap
    setMiniMapLocation(_settingsManager->getMiniMapLocation());
    setMiniMapStatus(_settingsManager->getMiniMapStatus());

    auto evaluateScenarioTitle = [](model::Graph* graph)
    {
        QString envName = "No Environment Selected";
        if(graph->getActiveEnvironment()!=nullptr)
        {
            envName = graph->getActiveEnvironment()->getName();
        }
        return QString("%1 [Env: %2]").arg(graph->getName(), envName);
    };

    _subwindowsMap.insert(sceneGraph->getModelGraph()->getIdentifier(), window);

    window->installEventFilter(new CloseEventIgnoreEventFilter());

    window->setWindowFlag(Qt::WindowType::WindowCloseButtonHint, true);
    window->setWindowFlag(Qt::WindowType::WindowShadeButtonHint, true);
    window->setWindowIcon(QIcon(":/ui/test_case"));
    window->setWindowTitle(evaluateScenarioTitle(sceneGraph->getModelGraph()));

    auto sysMenu = new QMenu(window);
    sysMenu->addAction(QIcon(":/ui/duplicate"), "Clone Scenario", [=]()
    {
        auto newName = QInputDialog::getText(this, "Clone Scenario", "New Scenario Name : ", QLineEdit::Normal, QString("%1 - Clone").arg(sceneGraphWidget->getSceneGraph()->getModelGraph()->getName()));
        if(!newName.isEmpty())
        {
            cloneScenario(sceneGraphWidget->getSceneGraph(), newName);
        }
    });

    sysMenu->addAction(QIcon(":/ui/delete"), "Delete Scenario", [=]()
    {
        deleteScenario(sceneGraphWidget->getSceneGraph());
    });
    window->setSystemMenu(sysMenu);

    connect(sceneGraph->getModelGraph(), &model::IdentifiableEntity::nameChanged, window, [=](const QString& str)
    {
        window->setWindowTitle(evaluateScenarioTitle(sceneGraph->getModelGraph()));
    });

    connect(sceneGraph->getModelGraph(), &model::Graph::activeEnvironmentChanged, window, [=]()
    {
        window->setWindowTitle(evaluateScenarioTitle(sceneGraph->getModelGraph()));
    });

    window->showMaximized();

    return sceneGraphWidget;
}

void MainWindow::cloneScenario(view::SceneGraph* sceneGraph, QString newName)
{
    auto sgw = (SceneGraphWidget*)sceneGraph->views().first();
    model::PersistenceHandler persistenceHandler;
    auto jsValue = sgw->saveToJSValue(&persistenceHandler);
    auto newGraph = sceneGraph->getModelGraph()->clone();
    newGraph->setName(newName);
    auto newSceneGraph = new view::SceneGraph(newGraph);
    auto newSGW = openScenario(newSceneGraph);
    newSGW->loadFromJSValue(jsValue);
}

void MainWindow::deleteScenario(view::SceneGraph* sceneGraph)
{
    auto choice = QMessageBox::warning(this, "Delete Scenarion", QString("You are about to delete (%1), proceed ?").arg(sceneGraph->getModelGraph()->getName()), QMessageBox::Yes, QMessageBox::Cancel);
    if(choice == QMessageBox::Yes)
    {
        delete sceneGraph->getModelGraph();
        auto graphWidget = sceneGraph->views().first();
        delete graphWidget->parent();
    }
}

void MainWindow::deleteActiveScenario()
{
    //for(auto subWindow : _ui.mdiArea->subWindowList())
    {
        auto sceneGraphWidget = dynamic_cast<SceneGraphWidget*>(_ui.mdiArea->activeSubWindow()->widget());
        if(sceneGraphWidget != nullptr)
        {
            deleteScenario(sceneGraphWidget->getSceneGraph());
        }
    }
}

void MainWindow::cloneActiveScenario()
{

    auto sceneGraphWidget = dynamic_cast<SceneGraphWidget*>(_ui.mdiArea->activeSubWindow()->widget());
    if(sceneGraphWidget != nullptr)
    {
        auto newName = QInputDialog::getText(this, "Clone Scenario", "New Scenario Name : ", QLineEdit::Normal, QString("%1 - Clone").arg(sceneGraphWidget->getSceneGraph()->getModelGraph()->getName()));
        if(!newName.isEmpty())
        {
            cloneScenario(sceneGraphWidget->getSceneGraph(), newName);
        }
    }
}


QJSValue MainWindow::savetoJSValue(model::PersistenceHandler* handler) const
{
    auto sceneGraphWidgets = findChildren<SceneGraphWidget*>();
    auto uiValue = handler->createJsValue();
    auto scenesValue = handler->createJsValue();

    std::for_each(sceneGraphWidgets.begin(), sceneGraphWidgets.end(), [=, &scenesValue](SceneGraphWidget* sgw)
    {
        auto sceneGraph = sgw->getSceneGraph();
        scenesValue.setProperty(sceneGraph->getModelGraph()->getIdentifier(), sgw->saveToJSValue(handler));
    });

    uiValue.setProperty("scenes", scenesValue);
    return uiValue;
}

bool MainWindow::loadFromJSValue(const QJSValue& v)
{
    //auto sceneGraphWidgets = findChildren<SceneGraphWidget*>();

    auto scenesValue = v.property("scenes");
    QJSValueIterator it(scenesValue);
    while (it.hasNext())
    {
        it.next();
        auto sceneId = it.name();
        auto sceneValue = it.value();
        auto sgw = findChild<SceneGraphWidget*>(sceneId);
        if (sgw != nullptr)
        {
            sgw->loadFromJSValue(sceneValue);
        }
    }

    return true;
}

void MainWindow::openLastProject()
{
    auto recentProjects = _settingsManager->enumRecentProjects();
    if (!recentProjects.isEmpty())
    {
        openProject(recentProjects.first());
    }
}

void MainWindow::setMiniMapLocation(int location)
{
    _settingsManager->setMiniMapLocation(location);
    for(auto subWindow : _ui.mdiArea->subWindowList())
    {
        auto sgw = dynamic_cast<SceneGraphWidget*>(subWindow->widget());
        if(sgw != nullptr)
        {
            sgw->getSceneGraph()->getMiniMap()->setAnchor(location);
        }
    }
}

void MainWindow::setMiniMapStatus(bool status)
{
    _settingsManager->setMiniMapStatus(status);
    auto subwindowsList = _ui.mdiArea->subWindowList();

    std::for_each(subwindowsList.begin(), subwindowsList.end(), [status](QMdiSubWindow* subWindow) {
        auto sgw = dynamic_cast<SceneGraphWidget*>(subWindow->widget());
        if(sgw != nullptr)
        {
            sgw->getSceneGraph()->getMiniMap()->setVisible(status);
        }
    });
    _miniMapAction->setChecked(status);
}

void MainWindow::setTheme(view::SettingsManager::Theme theme)
{
    _settingsManager->setTheme(theme);
    switch(theme) {
    case view::SettingsManager::Theme::LIGHT:
        qApp->setPalette(_lightPalette);
        break;
    default:
        qApp->setPalette(_darkPalette);
        break;
    }
}


void MainWindow::switchTheme()
{
    qDebug() << _settingsManager->getTheme();
    switch(_settingsManager->getTheme()) {
    case view::SettingsManager::Theme::LIGHT:
        setTheme(view::SettingsManager::Theme::DARK);
        break;
    case view::SettingsManager::Theme::DARK:
        setTheme(view::SettingsManager::Theme::LIGHT);
        break;
    }
}

void MainWindow::preparePalettes()
{
    // Light Palette
    _lightPalette = qApp->style()->standardPalette();

    // Dark Palette
    _darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    _darkPalette.setColor(QPalette::WindowText, Qt::white);
    _darkPalette.setColor(QPalette::Base, QColor(15,15,15));
    _darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    _darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    _darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    _darkPalette.setColor(QPalette::Text, Qt::white);
    _darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    _darkPalette.setColor(QPalette::ButtonText, Qt::white);
    _darkPalette.setColor(QPalette::BrightText, Qt::red);

    _darkPalette.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
    _darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    _darkPalette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
    _darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
}

SceneGraphWidget *MainWindow::getActiveSceneGraphWidget() const
{
    if(_ui.mdiArea->activeSubWindow() == nullptr) return nullptr;
    return dynamic_cast<SceneGraphWidget*>(_ui.mdiArea->activeSubWindow()->widget());
}

void MainWindow::onOpenProject()
{
    auto fileName = QFileDialog::getOpenFileName(this, "Open project", _settingsManager->getLastOpenedLocation(), "RQFL Project (*.rqfl)");
    openProject(fileName);
}

void MainWindow::updateRecentProjectsList()
{
    auto recentProjects = _settingsManager->enumRecentProjects();
    //auto menu = _openProject->menu();
    auto menu = _recentProjectsMenu;
    auto actions = menu->actions();

    for (auto action : actions)
    {
        menu->removeAction(action);
        delete action;
    }

    // add an open project action
    for (const auto& prj : recentProjects)
    {
        auto action = menu->addAction(QIcon(":/ui/network"), prj);
        connect(action, &QAction::triggered, this, [=]()
        {
            openProject(prj);
        });
    }

    if (recentProjects.size() > 0)
    {
        menu->addSeparator();
        auto action = menu->addAction("Clear recent projects list");
        auto font = action->font();
        font.setBold(true);
        action->setFont(font);
        action->setIcon(QIcon(":/ui/broom"));
        connect(action, &QAction::triggered, this, [=]()
        {
            if (QMessageBox::warning(this,
                                     "Warning",
                                     "Clear recent projects list",
                                     QMessageBox::Yes, QMessageBox::Cancel) == QMessageBox::Yes)
            {
                _settingsManager->clearRecentProjects();
                updateRecentProjectsList();
            }
        });
    }
    else
    {
        auto action = menu->addAction("Empty list");
        action->setEnabled(false);
    }
}

int MainWindow::onCloseProject()
{
    int button = -999;
    if (_project != nullptr)
    {
        button = QMessageBox::information(this, "Save", "Save the project before closing it?",
                                          QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        switch (button)
        {
        case QMessageBox::StandardButton::Yes:
            onSaveProject();
            break;
        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            return -1;
        }

        for(auto graph : _project->getGraphs())
        {
            graph->cancel();
        }
    }

    setWindowTitle("RequestFlow");

    _project.reset();

    qDeleteAll(_ui.mdiArea->subWindowList());
    _ui.mdiArea->closeAllSubWindows();

    _ui.environmentsWidget->setProject(nullptr);
    _ui.environmentsWidget->update();

    setProject(nullptr);
    return button;
}

void MainWindow::onSaveProject()
{
    if (_project != nullptr)
    {
        QString path = _project->getPath();
        if (path.isEmpty())
        {
            path = QFileDialog::getSaveFileName(this, "Save project", "", "RQFL Project (*.rqfl)");
            if (path.isEmpty()) return;
        }
        QFile fp(path);
        if (fp.open(QIODevice::WriteOnly))
        {
            model::PersistenceHandler handler;
            _project->setPath(path);
            auto projectValue = _project->saveToJSValue(&handler);
            auto uiValue = savetoJSValue(&handler);
            projectValue.setProperty("ui", uiValue);

            QTextStream out(&fp);
            out << handler.evaluate(projectValue);
            fp.close();
        }
    }
}

void MainWindow::onSaveProjectAs()
{
    if (_project != nullptr)
    {
        QString path = QFileDialog::getSaveFileName(this, "Save project As ...", _project->getPath(), "RQFL Project (*.rqfl)");
        if (path.isEmpty()) return;

        QFile fp(path);
        if (fp.open(QIODevice::WriteOnly))
        {
            model::PersistenceHandler handler;
            _project->setPath(path);
            auto projectValue = _project->saveToJSValue(&handler);
            auto uiValue = savetoJSValue(&handler);
            projectValue.setProperty("ui", uiValue);

            QTextStream out(&fp);
            out << handler.evaluate(projectValue);
            fp.close();
        }
    }
}

void MainWindow::onImportSwagger()
{
    if (_project)
    {
        auto fileName = QFileDialog::getOpenFileName(this, "Import swagger file", "", "JSON (*.json)");

        if (!fileName.isEmpty())
        {
            auto document = new model::Document(_project.get());
            if (!document->importFromSwagger(fileName))
            {
                delete document;
            }
            else
            {
                _ui.inventoryWidget->setProject(_project.get());
            }
        }
    }
}

void MainWindow::onNewProject()
{
    if(onCloseProject() != -1)
    {
        setProject(new model::Project(this));
    }
}

void MainWindow::onSceneDeleted(QString identifier)
{
    if (_subwindowsMap.contains(identifier))
    {
        delete _subwindowsMap.take(identifier);
    }
}

void MainWindow::onActivateScene(model::Graph* scene)
{
    if (_subwindowsMap.contains(scene->getIdentifier()))
    {
        _subwindowsMap[scene->getIdentifier()]->showMaximized();
    }
}
