#include "externalnodeselectiondialog.h"
#include "ui_externalnodeselectiondialog.h"

#include <QDebug>
#include <model/Graph.h>
#include <QAbstractItemModel>

class CustomGraphModel : public QAbstractItemModel
{
public:
    CustomGraphModel(model::Graph* current, QObject* parent) : QAbstractItemModel(parent)
    {
        _filteredGraphs = filterGraphs(current);
    }

    QList<model::Graph*> filterGraphs(model::Graph* current)
    {
        QList<model::Graph*> graphs;
        for(auto gr : current->getProject()->getGraphs())
        {
            if(gr != current)
                graphs << gr;
        }
        return graphs;
    }

    QModelIndex index(int row, int column, const QModelIndex &parent) const override
    {
        return createIndex(row, column, _filteredGraphs[row]);
    }

    QModelIndex parent(const QModelIndex &child) const override
    {
        return QModelIndex();
    }

    int rowCount(const QModelIndex &parent) const override
    {
        return _filteredGraphs.size();
    }

    int columnCount(const QModelIndex &parent) const override
    {
        return 1;
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if(!hasIndex(index.row(), index.column(), index.parent())) return QVariant();

        if(role == Qt::DisplayRole)
        {
            auto graph = _filteredGraphs[index.row()];
            return graph->getName();
        }
        return QVariant();
    }
private:
    QList<model::Graph*> _filteredGraphs;
};

class CustomNodeModel : public QAbstractItemModel
{
public:
    CustomNodeModel(model::Graph* graph, const QString& filter, QObject* parent) : QAbstractItemModel(parent), _graph(graph)
    {
        _filteredNodes = filterNodes(filter);
    }

    QList<model::Node*> filterNodes(const QString& filter = "")
    {
        QList<model::Node*> nodes;
        for(auto node :_graph->getNodes())
        {
            if(node->getOutputSlots().size() > 0) // can output a message
            {
                if(node->getName().toLower().contains(filter) ||
                        node->getType().toLower().contains(filter))
                {
                    nodes << node;
                }
            }
        }
        return nodes;
    }

    QModelIndex index(int row, int column, const QModelIndex &parent) const override
    {
        return createIndex(row, column, _filteredNodes[row]);
    }

    QModelIndex parent(const QModelIndex &child) const override
    {
        return QModelIndex();
    }

    int rowCount(const QModelIndex &parent) const override
    {
        return _filteredNodes.size();
    }

    int columnCount(const QModelIndex &parent) const override
    {
        return 1;
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if(!hasIndex(index.row(), index.column(), index.parent())) return QVariant();

        if(role == Qt::DisplayRole)
        {
            auto node = _filteredNodes[index.row()];
            return QString("%1[%2]").arg(node->getType()).arg(node->getName());
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        if(role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal)
        {
            return "NodeType[Node name]";
        }
        return QVariant();
    }
private:
    model::Graph* _graph = nullptr;
    QList<model::Node*> _filteredNodes;
};


ExternalNodeSelectionDialog::ExternalNodeSelectionDialog(model::Graph* current, QWidget *parent) :
    QDialog(parent)
{
    _ui.setupUi(this);

    connect(_ui.lineEdit_filter, &QLineEdit::textChanged,this, [=](const QString& filter)
    {
        auto index = _ui.listView_graphs->currentIndex();
        if(index.isValid())
        {
            auto graph = reinterpret_cast<model::Graph*>(index.internalPointer());
            fillNodes(graph, filter);
        }
    });

    connect(_ui.tableView_nodes, &QTableView::pressed, this, [=](const QModelIndex& index)
    {
        _selectedNode = nullptr;
        if(index.isValid())
        {
            auto node = reinterpret_cast<model::Node*>(index.internalPointer());
            if(node != nullptr)
            {
                _selectedNode = node;
            }
        }
    });

    connect(_ui.listView_graphs, &QListView::pressed, this, [=](const QModelIndex& index)
    {
        if(index.isValid())
        {
            auto graph = reinterpret_cast<model::Graph*>(index.internalPointer());
            if(graph != nullptr)
            {
                fillNodes(graph, _ui.lineEdit_filter->text());
            }
        }
    });
    setWindowFlag(Qt::WindowType::WindowContextHelpButtonHint, false);

    fillGraphs(current);
}

ExternalNodeSelectionDialog::~ExternalNodeSelectionDialog()
{
}

model::Node* ExternalNodeSelectionDialog::getSelectedNode() const
{
    return _selectedNode;
}

void ExternalNodeSelectionDialog::fillGraphs(model::Graph* graph)
{
    auto model = _ui.listView_graphs->model();
    if(model != nullptr)
    {
        delete model;
    }
    model = new CustomGraphModel(graph, this);
    _ui.listView_graphs->setModel(model);
    _ui.listView_graphs->setCurrentIndex(_ui.listView_graphs->rootIndex());
    _ui.listView_graphs->setCurrentIndex(model->index(0, 0));
}

void ExternalNodeSelectionDialog::fillNodes(model::Graph *graph, const QString &filter)
{
    auto model = _ui.tableView_nodes->model();
    if(model != nullptr)
    {
        delete model;
    }
    model = new CustomNodeModel(graph, filter, this);
    _ui.tableView_nodes->setModel(model);
    _ui.tableView_nodes->setCurrentIndex(model->index(0, 0));
}
