#include "ScenariosWidget.h"
#include <QDebug>
#include <QAbstractItemModel>
#include <QStringListModel>

class ScenariosModel : public QAbstractItemModel
{
public:
	ScenariosModel(QObject* parent) : QAbstractItemModel(parent), _scenarioIcon(QIcon(":/ui/test_case")) {}

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override
	{
		if (!hasIndex(row, column, parent)) return QModelIndex();

		return createIndex(row, column, _project->getGraphs().at(row));
	}

	QModelIndex parent(const QModelIndex& child) const override
	{
		return QModelIndex();
	}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override
	{
		if (_project == nullptr) return 0;
		return _project->getGraphs().size();
	}

	int columnCount(const QModelIndex& parent = QModelIndex()) const override
	{
		return 1;
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override
	{
		if (!index.isValid()) return QVariant();

		auto graph = reinterpret_cast<model::Graph*>(index.internalPointer());
		switch (role)
		{
		case Qt::DecorationRole:
			return _scenarioIcon;
		case Qt::EditRole:
		case Qt::DisplayRole:
			return QVariant(graph->getName());
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
	QIcon _scenarioIcon;
};


ScenariosWidget::ScenariosWidget(QWidget *parent) : QWidget(parent)
{
	_ui.setupUi(this);
	_scenariosModel = new ScenariosModel(this);

	_ui.listView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(_ui.listView, &QWidget::customContextMenuRequested, this, &ScenariosWidget::onContextMenuRequested);

	_ui.listView->setModel(_scenariosModel);
}

ScenariosWidget::~ScenariosWidget()
{
}

void ScenariosWidget::setProject(model::Project* project)
{
	_project = project;
	_scenariosModel->setProject(project);
}

model::Project* const ScenariosWidget::getProject() const
{
	return _project;
}

void ScenariosWidget::setCurrentSceneGraph(view::SceneGraph* sceneGraph)
{
	_currentSceneGraph = sceneGraph;
	//TODO: select the correct item in the list
	emit currentScenarioChanged(sceneGraph);
}

view::SceneGraph* const ScenariosWidget::getCurrentSceneGraph() const
{
	return _currentSceneGraph;
}

void ScenariosWidget::fillScenariosList()
{
	qDebug() <<__FUNCTION__ << _project->getGraphs().size();

	_scenariosModel->beginResetModel();
	_scenariosModel->endResetModel();
	_ui.listView->update();
	update();
}

void ScenariosWidget::onContextMenuRequested(const QPoint& p)
{
	
	fillScenariosList();
	auto index = _ui.listView->currentIndex();
	if (index.isValid())
	{
	}	
}
