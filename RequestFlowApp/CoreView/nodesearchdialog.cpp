#include "NodeSearchDialog.h"
#include "ui_NodeSearchDialog.h"

view::NodeSearchDialog::NodeSearchDialog(SceneGraphWidget* sgw) :
      QDialog(sgw->parentWidget()),
      ui(new Ui::NodeSearchDialog),
      _sgw(sgw)
{
    ui->setupUi(this);
    ui->listView->setModel(new CustomNodeModel(sgw->getSceneGraph()->getModelGraph(), "", this));
    connect(ui->toolButton, &QToolButton::clicked, this, [=]()
            {
                auto mdl = dynamic_cast<CustomNodeModel*>(ui->listView->model());
                mdl->setFilter(ui->lineEdit->text());
            });

    connect(ui->lineEdit, &QLineEdit::textEdited, this, [=](const QString& txt)
            {
                auto mdl = dynamic_cast<CustomNodeModel*>(ui->listView->model());
                mdl->setFilter(txt);
            });

    connect(ui->listView, &QListView::pressed, this, &NodeSearchDialog::onNodeSelected);
    connect(ui->listView, &QListView::activated, this, &NodeSearchDialog::onNodeSelected);
    connect(ui->listView, &QListView::entered, this, &NodeSearchDialog::onNodeSelected);
}

view::NodeSearchDialog::~NodeSearchDialog()
{
    delete ui;
}

#include <QDebug>

void view::NodeSearchDialog::onNodeSelected(const QModelIndex &index)
{
    if(index.isValid())
    {
        auto node = static_cast<model::Node*>(index.internalPointer());
        auto grNode = _sgw->getSceneGraph()->findbyModel(node);
        _sgw->setCenterAnimated(grNode);
    }
}
