#pragma once

#include <QWidget>
//namespace Ui { class EnvironmentsWidget; };
#include "ui_EnvironmentsWidget.h"

#include <model/Graph.h>
#include <model/Environment.h>
#include <model/Project.h>

#include <QTimer>

class EnvironmentsModel;

class EnvironmentsWidget : public QWidget
{
	Q_OBJECT
public:
	EnvironmentsWidget(QWidget *parent = nullptr);

	void setActiveEnvironment(model::Environment* env);
	model::Environment* getActiveEnvironment() const;
	void setProject(model::Project* project);

protected slots:
	void onCreateEnvironment();
	void onCloneEnvironment();
	void onDeleteEnvironment();

	void onExportEnvironments();
	void onImportEnvironments();

	void copyEnvironmentEntriesFromUi();
	void copyEnvironmentEntriesToUi();

	void onSelectCurrentEnvironmentIndex(int index);

signals:
	void currentEnvironmentChanged();

private:
	Ui::EnvironmentsWidget _ui;
	model::Environment* _environment = nullptr;
	model::Project* _project = nullptr;
	EnvironmentsModel* _environmentsModel = nullptr;
};
