#pragma once

#include <QWidget>
#include "ui_InventoryWidget.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <model/Document.h>
#include <model/EndpointEntry.h>
#include <model/Project.h>
#include <QTimer>

#include "inventoryitemmodel.h"


class InventoryWidget : public QWidget
{
	Q_OBJECT

public:
	InventoryWidget(QWidget *parent = Q_NULLPTR);
	~InventoryWidget();

	void setProject(model::Project* project);
private slots:
	void onContextMenuRequested(const QPoint& p);
    void applyFilter(const QString& filter);

private:
    InventoryItem *createRootItem(model::Project *project);

private:
	Ui::InventoryWidget _ui;
	model::Project* _project = nullptr;
    InventoryItemModel* _model = nullptr;
    QTimer *_timer=nullptr;
};
