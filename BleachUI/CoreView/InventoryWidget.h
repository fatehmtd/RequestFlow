#pragma once

#include <QWidget>
#include "ui_InventoryWidget.h"

class InventoryWidget : public QWidget
{
	Q_OBJECT

public:
	InventoryWidget(QWidget *parent = Q_NULLPTR);
	~InventoryWidget();

private:
	Ui::InventoryWidget ui;
};
