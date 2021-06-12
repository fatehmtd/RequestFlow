#include "CustomNodeModel.h"

CustomNodeModel::CustomNodeModel(model::Graph *graph, const QString &filter, QObject *parent)
    : QAbstractItemModel(parent), _graph(graph)
{
    _filteredNodes = filterNodes(filter);
}

QList<model::Node *> CustomNodeModel::filterNodes(const QString &filter)
{
    auto lowerFilter = filter.toLower();
    QList<model::Node *> nodes;
    for (auto node : _graph->getNodes()) {
        if (node->getOutputSlots().size() > 0) // can output a message
        {
            if (node->getName().toLower().contains(lowerFilter)
                || node->getType().toLower().contains(lowerFilter)) {
                nodes << node;
            }
        }
    }
    return nodes;
}

QModelIndex CustomNodeModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column, _filteredNodes[row]);
}

QModelIndex CustomNodeModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int CustomNodeModel::rowCount(const QModelIndex &parent) const
{
    return _filteredNodes.size();
}

int CustomNodeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant CustomNodeModel::data(const QModelIndex &index, int role) const
{
    if (!hasIndex(index.row(), index.column(), index.parent()))
        return QVariant();

    auto node = _filteredNodes[index.row()];

    switch(role)
    {
    case Qt::DisplayRole:
    {
        QString sanitizedName = node->getName().isEmpty() ? "Untitled" : node->getName();
        return QString("%1[%2]").arg(node->getType(), sanitizedName);
    }
    break;
    }
    return QVariant();
}

QVariant CustomNodeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal) {
        return "NodeType[Node name]";
    }
    return QVariant();
}

void CustomNodeModel::setFilter(const QString &filter)
{
    beginResetModel();
    _filter = filter;
    _filteredNodes = filterNodes(filter);
    endResetModel();
}

QString CustomNodeModel::getFilter() const
{
    return _filter;
}
