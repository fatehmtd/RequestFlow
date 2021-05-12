#include "EnvironmentsWidget.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QDebug>

#include <QAbstractItemModel>
#include <QStringListModel>

class EnvironmentsModel : public QAbstractItemModel
{
public:
	EnvironmentsModel(QObject* parent) : QAbstractItemModel(parent), _envIcon(QIcon((":/ui/environment"))) {}

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override
	{
		if (!hasIndex(row, column, parent)) return QModelIndex();

		return createIndex(row, column, _project->getEnvironments().at(row));
	}

	QModelIndex parent(const QModelIndex& child) const override
	{
		return QModelIndex();
	}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override
	{
		if (_project == nullptr) return 0;
		return _project->getEnvironments().size();
	}

	int columnCount(const QModelIndex& parent = QModelIndex()) const override
	{
		return 1;
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override
	{
		if (!index.isValid()) return QVariant();

		auto env = reinterpret_cast<model::Environment*>(index.internalPointer());
		switch (role)
		{
		case Qt::DecorationRole:
			return _envIcon;
		case Qt::EditRole:
		case Qt::DisplayRole:
			return QVariant(env->getName());
		}

		return QVariant();
	}

	void setProject(model::Project* project)
	{
		beginResetModel();
		_project = project;
		endResetModel();
	}

	void beginResetModel()
	{
		QAbstractItemModel::beginResetModel();
	}

	void endResetModel()
	{
		QAbstractItemModel::endResetModel();
	}

private:
	model::Project* _project = nullptr;
	QIcon _envIcon;
};

EnvironmentsWidget::EnvironmentsWidget(QWidget* parent) : QWidget(parent)
{
	_ui.setupUi(this);
	_environmentsModel = new EnvironmentsModel(this);
	setWindowIcon(QIcon(":/ui/environment"));

	auto cloneAction = new QAction(QIcon(":/BleachUI/copy"), "Clone");
	connect(cloneAction, &QAction::triggered, this, &EnvironmentsWidget::onCloneEnvironment);
	_ui.toolButton_add->addAction(cloneAction);
	_ui.toolButton_add->setPopupMode(QToolButton::ToolButtonPopupMode::MenuButtonPopup);

	connect(_ui.toolButton_add, &QAbstractButton::clicked, this, &EnvironmentsWidget::onCreateEnvironment);
	connect(_ui.toolButton_remove, &QAbstractButton::clicked, this, &EnvironmentsWidget::onDeleteEnvironment);

	_ui.tableWidget_entries->setRowCount(100);

	_ui.comboBox_environments->setIconSize(QSize(16, 16));

	connect(_ui.comboBox_environments, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectCurrentEnvironmentIndex(int)));

	connect(_ui.tableWidget_entries, &QTableWidget::itemChanged, this, [=](QTableWidgetItem* item)
		{
			if (_environment == nullptr) return;

			_ui.tableWidget_entries->blockSignals(true);
			copyEnvironmentEntriesFromUi();
			_ui.tableWidget_entries->blockSignals(false);
		});
}

void EnvironmentsWidget::setActiveEnvironment(model::Environment* env)
{
	_environment = env;

	_ui.tableWidget_entries->setRowCount(0);
	_ui.tableWidget_entries->setRowCount(100);
	if (_environment != nullptr)
	{
		copyEnvironmentEntriesToUi();
	}
}

model::Environment* EnvironmentsWidget::getActiveEnvironment() const
{
	return _environment;
}

void EnvironmentsWidget::setProject(model::Project* project)
{
	_project = project;

	// fill the combobox
	if (project != nullptr)
	{
		auto environments = _project->getEnvironments();
		_ui.comboBox_environments->setModel(_environmentsModel);
		_ui.comboBox_environments->update();
		_environmentsModel->setProject(project);
	}
	else
	{
		_environmentsModel->setProject(project);
		_ui.comboBox_environments->setModel(nullptr);
		_ui.comboBox_environments->clear();
		_ui.tableWidget_entries->clearContents();
	}
}

void EnvironmentsWidget::onCreateEnvironment()
{
	auto name = QInputDialog::getText(this, "Create Environment", "New Environment name :");

	if (!name.isEmpty()) //TODO: validate the name
	{
		//_ui.comboBox_environments->clear();
		_environmentsModel->beginResetModel();
		auto environment = new model::Environment(_project);
		environment->setName(name);
		_environmentsModel->endResetModel();

		int index = _project->getEnvironments().indexOf(environment);
		_ui.comboBox_environments->setCurrentIndex(index);
	}
}

void EnvironmentsWidget::onCloneEnvironment()
{

	QString tempName = QString("%1 (Clone)").arg(_environment->getName()); //TODO: generate valid name
	auto name = QInputDialog::getText(this, "Create Environment", "New Environment name :", QLineEdit::EchoMode::Normal, tempName);
	if (!name.isEmpty()) //TODO: validate the name
	{
		_environmentsModel->beginResetModel();

		auto newEnv = new model::Environment(*_environment);
		newEnv->setEntries(_environment->getEntries());
		newEnv->setName(name);

		_environmentsModel->endResetModel();
		int index = _project->getEnvironments().indexOf(newEnv);
		_ui.comboBox_environments->setCurrentIndex(index);
	}
}

void EnvironmentsWidget::onDeleteEnvironment()
{
	auto envList = _project->getEnvironments();
	if (envList.size() > 1)
	{
		if (QMessageBox::warning(this, QString("Delete (%1)").arg(_environment->getName()), "Proceed ?", QMessageBox::Yes, QMessageBox::Cancel)
			== QMessageBox::Yes)
		{
			_environmentsModel->beginResetModel();
			delete _environment;
			_environment = nullptr;
			_environmentsModel->endResetModel();
		}
	}
}

void EnvironmentsWidget::onExportEnvironments()
{
}

void EnvironmentsWidget::onImportEnvironments()
{
}

void EnvironmentsWidget::copyEnvironmentEntriesFromUi()
{
	QMap<QString, QVariant> entries;

	for (int i = 0; i < _ui.tableWidget_entries->rowCount(); i++)
	{
		auto keyItem = _ui.tableWidget_entries->item(i, 0);
		if (keyItem == nullptr) continue;
		if (keyItem->text().isEmpty()) continue;

		auto valueItem = _ui.tableWidget_entries->item(i, 1);
		QString value = valueItem == nullptr ? "" : valueItem->text();
		entries[keyItem->text()] = value;
	}

	if (!entries.isEmpty())
	{
		_environment->setEntries(entries);
	}
}

void EnvironmentsWidget::copyEnvironmentEntriesToUi()
{
	if (!_environment->getEntries().isEmpty())
	{
		_ui.tableWidget_entries->blockSignals(true);

		const auto& entries = _environment->getEntries().keys();

		int index = 0;
		std::for_each(entries.begin(), entries.end(), [=, &index](const QString& key)
			{
				_ui.tableWidget_entries->setItem(index, 0, new QTableWidgetItem(key));
				_ui.tableWidget_entries->setItem(index, 1, new QTableWidgetItem(_environment->getEntries().value(key).toString()));
				++index;
			});
		_ui.tableWidget_entries->blockSignals(false);
	}
}

void EnvironmentsWidget::onSelectCurrentEnvironmentIndex(int index)
{
	auto env = reinterpret_cast<model::Environment*>(_environmentsModel->index(index, 0).internalPointer());
	if (env != nullptr)
	{
		setActiveEnvironment(env);

		emit currentEnvironmentChanged(env);
	}
}
