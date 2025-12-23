#include "ExternalNodeSelectionDialog.h"
#include "ui_ExternalNodeSelectionDialog.h"

#include <QDebug>
#include <model/Graph.h>
#include <QAbstractItemModel>

#include "CustomNodeModel.h"

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
        if(_filteredGraphs.isEmpty()) return QModelIndex();
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

    connect(_ui.tableView_nodes, &QTableView::activated, this, [=](const QModelIndex& index)
    {
        _selectedNode = nullptr;
        if(index.isValid())
        {
            auto node = reinterpret_cast<model::Node*>(index.internalPointer());
            if(node != nullptr)
            {
                _selectedNode = node;
                accept();
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

    connect(_ui.listView_graphs, &QListView::activated, this, [=](const QModelIndex& index)
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

#include <QItemSelectionModel>

void ExternalNodeSelectionDialog::fillGraphs(model::Graph* graph)
{
    auto model = _ui.listView_graphs->model();
    if(model != nullptr)
    {
        delete model;
    }
    model = new CustomGraphModel(graph, this);
    _ui.listView_graphs->setModel(model);
    _ui.listView_graphs->update();
    auto index = model->index(0, 0);

    if(index.isValid())
    {
        auto graph = reinterpret_cast<model::Graph*>(index.internalPointer());
        if(graph != nullptr)
        {
            fillNodes(graph, _ui.lineEdit_filter->text());
        }
    }
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
