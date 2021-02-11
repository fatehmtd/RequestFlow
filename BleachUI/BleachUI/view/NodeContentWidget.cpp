#include "NodeContentWidget.h"
#include "ui_NodeContentWidget.h"

view::NodeContentWidget::NodeContentWidget(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::NodeContentWidget();
	ui->setupUi(this);
}

view::NodeContentWidget::~NodeContentWidget()
{
	delete ui;
}
