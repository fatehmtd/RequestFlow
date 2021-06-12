#pragma once

#include <model/Project.h>
#include <QDialog>
#include "ui_ExternalNodeSelectionDialog.h"
#include <model/Node.h>


class ExternalNodeSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExternalNodeSelectionDialog(model::Graph* graph, QWidget *parent = nullptr);
    ~ExternalNodeSelectionDialog();

    model::Node* getSelectedNode() const;
private:
    void fillGraphs(model::Graph* graph);
    void fillNodes(model::Graph* graph, const QString& filter="");

private:
    Ui::ExternalNodeSelectionDialog _ui;
    model::Project* _project = nullptr;
    model::Node* _selectedNode = nullptr;
    model::Graph* _selectedGraph = nullptr;
};
