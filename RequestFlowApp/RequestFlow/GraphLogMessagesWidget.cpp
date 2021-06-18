#include "GraphLogMessagesWidget.h"
#include "LogMessagesWidget.h"
#include "view/Colors.h"

GraphLogMessagesWidget::GraphLogMessagesWidget(QWidget* parent, model::MessageLogger* logger) : QWidget(parent), _messageLogger(logger)
{
	ui.setupUi(this);
    connect(logger, &model::MessageLogger::cleared, this, &GraphLogMessagesWidget::onCleared);
    connect(logger, &model::MessageLogger::updated, this, &GraphLogMessagesWidget::onUpdated);
    connect(logger, &model::MessageLogger::logged, this, &GraphLogMessagesWidget::onLogged);

	connect(logger, &QObject::destroyed, this, &QObject::deleteLater);

    ui.progressBar->setFixedHeight(20);
	ui.progressBar->setVisible(false);

	auto graph = logger->getGraph();

    connect(graph, &model::Graph::preparingStartup, this, [=]()
            {
                ui.progressBar->setRange(0, graph->getNodes().size()-1);
                ui.progressBar->setValue(0);
                ui.progressBar->setVisible(false);
            });

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

    connect(this, &GraphLogMessagesWidget::senderSelected, (LogMessagesWidget*)parent, &LogMessagesWidget::senderSelected);

	connect(ui.tableWidget, &QTableWidget::itemSelectionChanged, this, [=]() 
		{
			//if (ui.tableWidget->rowCount() > 1)
			{
				auto data = ui.tableWidget->item(ui.tableWidget->currentRow(), 0)->data(Qt::UserRole + 1);
				auto sender = data.value<model::Node*>();
				if (sender != nullptr)
				{
					emit senderSelected(sender);
				}
            }
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
    //ui.progressBar->setVisible(false);
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
	auto messageItem = new QTableWidgetItem(m.message);

	QMap<QString, QColor> colorsMap;
	colorsMap["Assertion"] = view::colors::vividBurgundy;
	colorsMap["Delay"] = view::colors::yellow;
	colorsMap["Endpoint"] = view::colors::blue;
	colorsMap["Payload"] = view::colors::byzantium;
	colorsMap["Script"] = view::colors::charcoal;
	colorsMap["Viewer"] = view::colors::green;

	auto node = dynamic_cast<model::Node*>(m.sender);
	if (node)
	{
		typeItem->setData(Qt::UserRole + 1, QVariant::fromValue(m.sender));

		auto color = colorsMap[node->getType()];

		//if (node->getType() == "Endpoint")
		{
            messageItem->setData(Qt::BackgroundRole, color);
			messageItem->setData(Qt::ForegroundRole, QColor(255, 255, 255));
            dateTimeItem->setData(Qt::BackgroundRole, color);
			dateTimeItem->setData(Qt::ForegroundRole, QColor(255, 255, 255));
			//typeItem->setData(Qt::BackgroundColorRole, color);
			//typeItem->setData(Qt::ForegroundRole, QColor(255, 255, 255));
		}
	}

	ui.tableWidget->setItem(row, 0, typeItem);
	ui.tableWidget->setItem(row, 1, dateTimeItem);
	ui.tableWidget->setItem(row, 2, messageItem);
	//ui.tableWidget->scrollToBottom();
	ui.tableWidget->resizeRowsToContents();
}
