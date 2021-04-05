#include "EnvironmentsWidget.h"
#include <QInputDialog>
#include <QAction>
#include <QMenu>
#include <QDebug>

EnvironmentsWidget::EnvironmentsWidget(QWidget* parent) : QWidget(parent)
{
	_ui.setupUi(this);

	auto cloneAction = new QAction(QIcon(":/BleachUI/copy"), "Clone");
	connect(cloneAction, &QAction::triggered, this, &EnvironmentsWidget::onCloneEnvironment);
	_ui.toolButton_add->addAction(cloneAction);
	_ui.toolButton_add->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);

	connect(_ui.toolButton_add, &QAbstractButton::clicked, this, &EnvironmentsWidget::onCreateEnvironment);
	connect(_ui.toolButton_remove, &QAbstractButton::clicked, this, &EnvironmentsWidget::onDeleteEnvironment);

	_ui.tableWidget_entries->setRowCount(100);

	_timer.setSingleShot(true);
	_timer.setInterval(500);

	connect(_ui.tableWidget_entries, &QTableWidget::itemChanged, this, [=](QTableWidgetItem* item) 
		{
			if (_environment == nullptr) return;
			_timer.start();
		});

	connect(&_timer, &QTimer::timeout, this, [=]()
		{
			QMap<QString, QVariant> entries;

			for (int i = 0; i < _ui.tableWidget_entries->rowCount(); i++)
			{
				auto keyItem = _ui.tableWidget_entries->item(i, 0);
				if (keyItem == nullptr) continue;
				auto valueItem = _ui.tableWidget_entries->item(i, 1);
				QString value = valueItem == nullptr ? "" : valueItem->text();

				qDebug() << keyItem->text() << value;
				if (keyItem->text().isEmpty()) continue;
				entries[keyItem->text()] = value;
			}

			_environment->setEntries(entries);
		});
}

EnvironmentsWidget::~EnvironmentsWidget()
{
}

void EnvironmentsWidget::setActiveEnvironment(model::Environment* env)
{
	_environment = env;

	_ui.tableWidget_entries->setRowCount(0);
	_ui.tableWidget_entries->setRowCount(100);

	auto entries = env->getEntries().keys();

	int index = 0;
	std::for_each(entries.begin(), entries.end(), [=, &index](const QString& key)
		{
			_ui.tableWidget_entries->setItem(index, 0, new QTableWidgetItem(key));
			_ui.tableWidget_entries->setItem(index, 1, new QTableWidgetItem(env->getEntries().value(key).toString()));
			++index;
		});
}

model::Environment* EnvironmentsWidget::getActiveEnvironment() const
{
	return _environment;
}

void EnvironmentsWidget::setProject(model::Project* project)
{
	_project = project;

	// fill the combobox
	auto environments = _project->getEnvironments();
	
	_ui.comboBox_environments->clear();

	if (!environments.isEmpty())
	{
		for (auto env : environments)
		{
			_ui.comboBox_environments->addItem(env->getName());
		}
		setActiveEnvironment(environments.first());
	}
}

void EnvironmentsWidget::onCreateEnvironment()
{
}

void EnvironmentsWidget::onCloneEnvironment()
{
}

void EnvironmentsWidget::onDeleteEnvironment()
{
}

void EnvironmentsWidget::onExportEnvironments()
{
}

void EnvironmentsWidget::onImportEnvironments()
{
}
