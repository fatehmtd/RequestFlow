#pragma once

#include <QWidget>
#include "ui_GraphLogMessagesWidget.h"
#include <model/Graph.h>
#include <model/MessageLogger.h>

class GraphLogMessagesWidget : public QWidget
{
	Q_OBJECT
public:
	GraphLogMessagesWidget(QWidget *parent, model::MessageLogger* logger);
	~GraphLogMessagesWidget();
protected slots:
	void onCleared();
	void onUpdated();
	void onLogged(const model::MessageLogger::Message& m);
private:
	Ui::GraphLogMessagesWidget ui;
	model::MessageLogger* _messageLogger = nullptr;
};
