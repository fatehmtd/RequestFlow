#include "environmentsmodel.h"


EnvironmentsModel::EnvironmentsModel(QObject* parent) : QAbstractItemModel(parent), _envIcon(QIcon((":/ui/environment")))
{

}

QModelIndex EnvironmentsModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) return QModelIndex();

    return createIndex(row, column, _project->getEnvironments().at(row));
}

QModelIndex EnvironmentsModel::parent(const QModelIndex& child) const
{
    return QModelIndex();
}

int EnvironmentsModel::rowCount(const QModelIndex& parent) const
{
    if (_project == nullptr) return 0;
    return _project->getEnvironments().size();
}

int EnvironmentsModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant EnvironmentsModel::data(const QModelIndex& index, int role) const
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

void EnvironmentsModel::setProject(model::Project* project)
{
    beginResetModel();
    _project = project;
    endResetModel();
}

void EnvironmentsModel::beginResetModel()
{
    QAbstractItemModel::beginResetModel();
}

void EnvironmentsModel::endResetModel()
{
    QAbstractItemModel::endResetModel();
}
