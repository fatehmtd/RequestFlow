#include "InventoryWidget.h"

#include <QMenu>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include <QModelIndex>
#include <QMimeData>
#include <QDataStream>

InventoryWidget::InventoryWidget(QWidget* parent)
	: QWidget(parent)
{
	_ui.setupUi(this);
	_ui.treeView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(_ui.treeView, &QTreeView::customContextMenuRequested, this, &InventoryWidget::onContextMenuRequested);
	_ui.treeView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
	_ui.treeView->setDragEnabled(true);
	_ui.treeView->setDragDropMode(QAbstractItemView::DragDropMode::DragOnly);
    _model = new InventoryItemModel(this);
    _ui.treeView->setModel(_model);
	setProject(nullptr);

    connect(_ui.lineEdit, &QLineEdit::textEdited, this, &InventoryWidget::applyFilter);
}

InventoryWidget::~InventoryWidget()
{
}

void InventoryWidget::onContextMenuRequested(const QPoint& p)
{
    auto index = _ui.treeView->indexAt(p);
    if(!index.isValid()) return;
    auto inventoryItem = static_cast<InventoryItem*>(index.internalPointer());
    if(inventoryItem->getType() != InventoryItem::Type::Document) return;

    auto document = inventoryItem->getUserDataPtr<model::Document*>();

    if (document != nullptr)
    {
        auto menu = new QMenu(this);
        menu->addAction(QIcon(":/BleachUI/delete"), "Delete document", [=]()
            {
                int button = QMessageBox::warning(this, "Delete Document", QString("Confirm the deletion of {%1} ?").arg(document->getName()),
                    QMessageBox::Yes, QMessageBox::Cancel);
                switch (button)
                {
                case QMessageBox::StandardButton::Yes:
                    delete document;
                    setProject(_project);
                    break;
                case QMessageBox::Cancel:
                    return;
                }
            });

        menu->exec(_ui.treeView->mapToGlobal(p));
        menu->deleteLater();
    }
}

void InventoryWidget::applyFilter(const QString &filter)
{
    _model->setFilter(filter);
    _ui.treeView->collapseAll();

    if(_timer == nullptr)
    {
        _timer = new QTimer(this);
        _timer->setSingleShot(true);

        connect(_timer, &QTimer::timeout, this, [=]()
                {
                    int rows = _model->rowCount();
                    for(int i=0;i<rows;i++)
                    {
                        auto itemIndex = _model->index(i, 0, QModelIndex());
                        _ui.treeView->expand(itemIndex);
                    }
                });
    }

    if(!filter.isEmpty())
        _timer->start(300);
}

#include <CoreViewUtils.h>

InventoryItem *InventoryWidget::createRootItem(model::Project *project)
{
    static const QIcon icons[] = {
        view::SVGRenderer::getInstance()->renderToIcon(":/ui/get"),
        view::SVGRenderer::getInstance()->renderToIcon(":/ui/post") ,
        view::SVGRenderer::getInstance()->renderToIcon(":/ui/put") ,
        view::SVGRenderer::getInstance()->renderToIcon(":/ui/del"),
        view::SVGRenderer::getInstance()->renderToIcon(":/ui/patch") };

    const QIcon swaggerIcon = QIcon(":/ui/swagger");

    InventoryItem *rootItem = new InventoryItem();
    auto documents = project->getDocuments();
    std::for_each(documents.begin(), documents.end(), [=](const model::Document* document)
                  {
                      auto documentItem = new InventoryItem(InventoryItem::Type::Document, rootItem);
                      documentItem->setData(document->getName());
                      documentItem->setIcon(swaggerIcon);
                      documentItem->setUserDataPtr((void*)document);

                      auto endpoints = document->getEndpoints();

                      std::for_each(endpoints.begin(), endpoints.end(), [=](const model::EndpointEntry* entry)
                                    {
                                        auto entryItem = new InventoryItem(InventoryItem::Type::Endpoint, documentItem);
                                        entryItem->setIcon(icons[entry->getHttpMethod()]);
                                        entryItem->setData(entry->getUrl());
                                        entryItem->setUserDataPtr((void*)entry);
                                    });
                  });
    return rootItem;
}

void InventoryWidget::setProject(model::Project* project)
{
	_project = project;

	if (project != nullptr)
	{
        _model->setRootItem(createRootItem(project));
	}
    else
    {
        _model->setRootItem(nullptr);
    }

	_ui.treeView->update();
}
