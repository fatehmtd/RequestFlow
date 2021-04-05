#pragma once

#include <QWidget>
//namespace Ui { class EnvironmentsWidget; };
#include "ui_EnvironmentsWidget.h"

#include <model/Graph.h>
#include <model/Environment.h>
#include <model/Project.h>

#include <QTimer>

class EnvironmentsWidget : public QWidget
{
	Q_OBJECT

public:
	EnvironmentsWidget(QWidget *parent = Q_NULLPTR);
	~EnvironmentsWidget();

	void setActiveEnvironment(model::Environment* env);
	model::Environment* getActiveEnvironment() const;
	void setProject(model::Project* project);

protected slots:
	void onCreateEnvironment();
	void onCloneEnvironment();
	void onDeleteEnvironment();

	void onExportEnvironments();
	void onImportEnvironments();
private:
	Ui::EnvironmentsWidget _ui;
	model::Environment* _environment = nullptr;
	model::Project* _project = nullptr;
	QTimer _timer;
};
