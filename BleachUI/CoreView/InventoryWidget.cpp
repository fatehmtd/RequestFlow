#include "InventoryWidget.h"

InventoryWidget::InventoryWidget(QWidget* parent)
	: QWidget(parent)
{
	_ui.setupUi(this);
}

InventoryWidget::~InventoryWidget()
{
}

#include <QDebug>
#include <QStringList>

void InventoryWidget::setProject(model::Project* project)
{
	_project = project;

	if (project != nullptr)
	{
		auto documents = project->getDocuments();
		QStandardItemModel* model = new QStandardItemModel(this);

		//_ui.treeView->setHeaderHidden(true);
		QStringList labels;
		labels << "url" << "operation";
		model->setHorizontalHeaderLabels(labels);

		QIcon icons[] = {QIcon(":/ui/get"), QIcon(":/ui/post") , QIcon(":/ui/put") , QIcon(":/ui/del") };
		QIcon swaggerIcon = QIcon(":/ui/swagger");

		for (int i = 0; i < documents.size(); i++)
		{
			const auto& document = documents[i];
			auto documentItem = new QStandardItem(document->getName());
			documentItem->setData(swaggerIcon, Qt::ItemDataRole::DecorationRole);

			auto endpoints = document->getEndpoints();

			for (int j = 0; j < endpoints.size(); j++)
			{
				const auto& endpoint = endpoints[j];

				auto endpointItem = new QStandardItem(endpoint->getUrl());
				endpointItem->setData(icons[endpoint->getHttpMethod()], Qt::ItemDataRole::DecorationRole);

				auto test = new QStandardItem(endpoint->getIdentifier());

				QList<QStandardItem*> items;
				items << endpointItem << test;

				documentItem->appendRow(items);
			}

			QList<QStandardItem*> items;
			items << documentItem << new QStandardItem();

			model->appendRow(items);
		}
		_ui.treeView->setModel(model);
	}
	else
	{
		auto mdl = _ui.treeView->model();
		if (mdl != nullptr)
			mdl->deleteLater();
		_ui.treeView->setModel(nullptr);
	}
	_ui.treeView->update();
	update();
}
