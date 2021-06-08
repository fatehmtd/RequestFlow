#pragma once

#include <QDialog>
#include <model/Graph.h>
#include <model/Node.h>
#include "view/Node.h"
#include "view/SceneGraphWidget.h"
#include "view/SceneGraph.h"
#include "customnodemodel.h"

namespace Ui {
class NodeSearchDialog;
}

namespace view {

class NodeSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NodeSearchDialog(SceneGraphWidget* sgw);
    ~NodeSearchDialog();

private slots:
    void onNodeSelected(const QModelIndex& index);

private:
    Ui::NodeSearchDialog *ui;
    SceneGraphWidget *_sgw=nullptr;
};
} // namespace view
