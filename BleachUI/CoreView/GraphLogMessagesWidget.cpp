#include "GraphLogMessagesWidget.h"

GraphLogMessagesWidget::GraphLogMessagesWidget(QWidget* parent, model::MessageLogger* logger) : QWidget(parent), _messageLogger(logger)
{
	ui.setupUi(this);
	connect(logger, &model::MessageLogger::cleared, this, &GraphLogMessagesWidget::onCleared, Qt::ConnectionType::DirectConnection);
	connect(logger, &model::MessageLogger::updated, this, &GraphLogMessagesWidget::onUpdated, Qt::ConnectionType::DirectConnection);
	connect(logger, &model::MessageLogger::logged, this, &GraphLogMessagesWidget::onLogged, Qt::ConnectionType::DirectConnection);

	connect(logger, &QObject::destroyed, this, &QObject::deleteLater);

	ui.progressBar->setVisible(false);
	auto graph = logger->getGraph();

	connect(graph, &model::Graph::started, this, [=]()
		{
			ui.progressBar->setRange(0, graph->getNodes().size()-1);
			ui.progressBar->setValue(0);
			ui.progressBar->setVisible(true);
		});

	connect(graph, &model::Graph::advanced, this, [=]()
		{
			ui.progressBar->setValue(ui.progressBar->value() + 1);
		});
	connect(graph, &model::Graph::stopped, this, [=]()
		{
			ui.progressBar->setVisible(false);
		});
}

GraphLogMessagesWidget::~GraphLogMessagesWidget()
{
}

void GraphLogMessagesWidget::onUpdated()
{
}

void GraphLogMessagesWidget::onCleared()
{
	//ui.progressBar->setVisible(true);
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(0);
}

void GraphLogMessagesWidget::onLogged(const model::MessageLogger::Message& m)
{
	int row = ui.tableWidget->rowCount();
	ui.tableWidget->setRowCount(row + 1);

	static QString str[] = { "SUCCESS", "INFO", "WARN", "DEBUG", "ERROR" };
	static QIcon icons[] = { QIcon(":/ui/ok"), QIcon(":/ui/info"), QIcon(":/ui/warningyellow"), QIcon(":/ui/debug"), QIcon(":/ui/error") };

	auto typeItem = new QTableWidgetItem(icons[m.type], str[m.type]);
	auto dateTimeItem = new QTableWidgetItem(m.dateTime.toString("hh:mm:ss"));
	auto messateItem = new QTableWidgetItem(m.message);
	ui.tableWidget->setItem(row, 0, typeItem);
	ui.tableWidget->setItem(row, 1, dateTimeItem);
	ui.tableWidget->setItem(row, 2, messateItem);
	ui.tableWidget->scrollToBottom();
	ui.tableWidget->resizeRowsToContents();
}
