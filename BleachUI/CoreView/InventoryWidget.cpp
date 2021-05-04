#include "InventoryWidget.h"

#define MEMBER_PTR_ROLE Qt::UserRole + 1
//Q_DECLARE_METATYPE(model::Document*);
//Q_DECLARE_METATYPE(model::EndpointEntry*);

#include <QDebug>
#include <QStringList>
#include <QModelIndex>
#include <QMimeData>
#include <QDataStream>

class CustomModel : public QStandardItemModel
{
public:
	using QStandardItemModel::QStandardItemModel;

	QStringList mimeTypes() const override
	{
		QStringList output;
		output << "application/x-EndpointEntry";
		return output;
	}

	QMimeData* mimeData(const QModelIndexList& indexes) const override
	{
		QMimeData* mime = new QMimeData();
		QByteArray array;
		QDataStream out(&array, QIODevice::WriteOnly);
		QList<model::EndpointEntry*> endpoints;
		for (const auto& index : indexes)
		{
			auto entry = index.data(MEMBER_PTR_ROLE).value<model::EndpointEntry*>();
			if (entry)
			{
				endpoints << entry;
			}
		}
		endpoints = endpoints.toSet().toList();
		if (!endpoints.isEmpty())
		{
			out << endpoints.size();
			for (const auto& endpoint : endpoints)
			{
				out << reinterpret_cast<size_t>(endpoint);
			}
			mime->setData("application/x-EndpointEntry", array);
		}
		return mime;
	}
private:
	model::Project* _project = nullptr;
};

InventoryWidget::InventoryWidget(QWidget* parent)
	: QWidget(parent)
{
	_ui.setupUi(this);
	_ui.treeView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(_ui.treeView, &QTreeView::customContextMenuRequested, this, &InventoryWidget::onContextMenuRequested);
	_ui.treeView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
	_ui.treeView->setDragEnabled(true);
	_ui.treeView->setDragDropMode(QAbstractItemView::DragDropMode::DragOnly);
	setProject(nullptr);
}

InventoryWidget::~InventoryWidget()
{
}

#include <QMenu>
#include <QMessageBox>


void InventoryWidget::onContextMenuRequested(const QPoint& p)
{
	auto index = _ui.treeView->indexAt(p);
	auto zeroIndex = _ui.treeView->model()->index(index.row(), 0, index.parent());
	auto endpointEntry = zeroIndex.data(MEMBER_PTR_ROLE).value<model::EndpointEntry*>();
	auto document = zeroIndex.data(MEMBER_PTR_ROLE).value<model::Document*>();

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
		menu->exec(mapToGlobal(p));
		menu->deleteLater();
	}
}

void InventoryWidget::setProject(model::Project* project)
{
	_project = project;

	QStandardItemModel* model = new CustomModel(this);

	//_ui.treeView->setHeaderHidden(true);
	QStringList labels;
	labels << "Location" << "Operation name";
	model->setHorizontalHeaderLabels(labels);
	model->horizontalHeaderItem(0)->setIcon(QIcon(":/ui/url"));
	model->horizontalHeaderItem(1)->setIcon(QIcon(":/ui/hashtag"));

	if (project != nullptr)
	{
		auto documents = project->getDocuments();

		QIcon icons[] = { QIcon(":/ui/get"), QIcon(":/ui/post") , QIcon(":/ui/put") , QIcon(":/ui/del") };
		QIcon swaggerIcon = QIcon(":/ui/swagger");

		for (int i = 0; i < documents.size(); i++)
		{
			const auto& document = documents[i];
			auto documentItem = new QStandardItem(document->getName());
			documentItem->setData(swaggerIcon, Qt::ItemDataRole::DecorationRole);
			documentItem->setData(QVariant::fromValue(document), MEMBER_PTR_ROLE);

			auto endpoints = document->getEndpoints();

			for (int j = 0; j < endpoints.size(); j++)
			{
				const auto& endpoint = endpoints[j];
				
				auto urlItem = new QStandardItem(endpoint->getUrl());
				urlItem->setData(icons[endpoint->getHttpMethod()], Qt::ItemDataRole::DecorationRole);
				auto operationIdItem = new QStandardItem(endpoint->getIdentifier());

				urlItem->setData(QVariant::fromValue(endpoint), MEMBER_PTR_ROLE);
				operationIdItem->setData(QVariant::fromValue(endpoint), MEMBER_PTR_ROLE);

				QList<QStandardItem*> items;
				items << urlItem << operationIdItem;

				documentItem->appendRow(items);
			}

			QList<QStandardItem*> items;
			items << documentItem << new QStandardItem();

			model->appendRow(items);
		}
	}
	_ui.treeView->setModel(model);
	_ui.treeView->update();
}
