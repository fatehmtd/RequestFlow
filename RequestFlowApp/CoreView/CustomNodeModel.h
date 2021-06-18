#pragma once

#include <QAbstractItemModel>
#include <model/Node.h>
#include <model/Graph.h>

class CustomNodeModel : public QAbstractItemModel
{
public:
    CustomNodeModel(model::Graph* graph, const QString& filter, QObject* parent) ;    

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setFilter(const QString& filter);
    QString getFilter() const;

private:
    QList<model::Node*> filterNodes(const QString& filter = "");

    QColor getNodeColor(model::Node* node) const;

private:
    model::Graph* _graph = nullptr;
    QList<model::Node*> _filteredNodes;
    QString _filter;
};
