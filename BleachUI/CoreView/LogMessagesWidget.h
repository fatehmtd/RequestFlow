#pragma once

#include <QWidget>
#include "ui_LogMessagesWidget.h"
#include <model/Project.h>
#include <model/MessageLogger.h>

class LogMessagesWidget : public QWidget
{
	Q_OBJECT

public:
	LogMessagesWidget(QWidget *parent = Q_NULLPTR);
	~LogMessagesWidget();
	void setProject(model::Project* project);

	void addMessageLogger(model::MessageLogger* logger);

private:
	void clearChildren();

private:
	Ui::LogMessagesWidget ui;
	model::Project* _project = nullptr;
};
