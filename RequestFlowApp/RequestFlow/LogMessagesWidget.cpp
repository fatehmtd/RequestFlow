#include "LogMessagesWidget.h"

LogMessagesWidget::LogMessagesWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

LogMessagesWidget::~LogMessagesWidget()
{
}

#include <model/Graph.h>
#include <model/MessageLogger.h>
#include "GraphLogMessagesWidget.h"

void LogMessagesWidget::setProject(model::Project* project)
{
	_project = project;
	clearChildren();
	if (_project != nullptr)
	{
        for (const auto& graph : _project->getGraphs())
		{
			addMessageLogger(graph->getLogger());
		}
	}
}

void LogMessagesWidget::addMessageLogger(model::MessageLogger* logger)
{
	auto widget = new GraphLogMessagesWidget(this, logger);
	ui.tabWidget->addTab(widget, QIcon(":/ui/test_case"), logger->getGraph()->getName());

    connect(logger->getGraph(), &model::Graph::started, this, [=]()
		{
			ui.tabWidget->setCurrentWidget(widget);
		});

    connect(logger->getGraph(), &model::Graph::preparingStartup, this, [=]()
            {
                ui.tabWidget->setCurrentWidget(widget);
            });

    connect(logger->getGraph(), &model::Graph::stopped, this, [=]()
            {
                ui.tabWidget->setCurrentWidget(widget);
            });
}

void LogMessagesWidget::clearChildren()
{
	ui.tabWidget->clear();
}
