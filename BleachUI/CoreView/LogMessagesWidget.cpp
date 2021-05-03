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
		for (auto graph : _project->getGraphs()) 
		{
			addMessageLogger(graph->getLogger());
		}
	}
}

void LogMessagesWidget::addMessageLogger(model::MessageLogger* logger)
{
	auto widget = new GraphLogMessagesWidget(this, logger);
	ui.tabWidget->addTab(widget, logger->getGraph()->getName());
}

void LogMessagesWidget::clearChildren()
{
	ui.tabWidget->clear();
}
