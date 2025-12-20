#include "PayloadNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Slot.h>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTableWidgetItem>

logic::PayloadNode::PayloadNode(model::PayloadNode* modelNode) : view::Node(modelNode, "Payload")
{
	setupUi();
}

model::Message logic::PayloadNode::composeMessage() const
{
	model::Message message;
	message.setBody(_ui.plainTextEdit_body->toPlainText());
	message.setPathVars(fillFromTable(_ui.tableWidget_path));
	message.setQueryParams(fillFromTable(_ui.tableWidget_query));
	return message;
}

int getNextDelimiterIndex(int offset, int minGap, const QStringList& delimiters, const QString& data)
{
    while(true)
    {
        // get the next delimiter index
        int furthest = -1;
        for(const auto& delim : delimiters)
        {
            int index = data.indexOf(delim, offset);
            if(index > furthest)
                furthest = index;
        }
        if(furthest == -1) return -1;
        if(furthest-offset >= minGap) return furthest;
        offset = furthest+1;
    }
    return -1;
}

extern void customDivideText(const QString& data, QTextCursor textCursor, int minChunkLen)
{
    textCursor.movePosition(QTextCursor::MoveOperation::Start, QTextCursor::MoveMode::MoveAnchor);
    textCursor.movePosition(QTextCursor::MoveOperation::End, QTextCursor::MoveMode::KeepAnchor);
    textCursor.removeSelectedText();

    if(true)
    {
        textCursor.beginEditBlock();
        textCursor.insertText(data);
        textCursor.endEditBlock();
    }
    else
    {
        const int totalLen = data.size();

        QStringList delimiters;
        delimiters << "}" << "]" << "\n";

        textCursor.beginEditBlock();
        int offset = 0;
        bool keepGoing = true;
        while(keepGoing)
        {
            // get the next delimiter index
            int furthest = getNextDelimiterIndex(offset, minChunkLen, delimiters, data);

            // if index is -1 then no further delimiters found, then almost or already at the end of the file
            if(furthest == -1 || furthest >= (totalLen-1))
            {
                furthest = totalLen-1;
                keepGoing = false;
            }

            if(furthest!= offset)
            {
                auto substring = data.mid(offset, furthest);
                textCursor.insertBlock();
                textCursor.insertText(substring);
                offset = furthest+1;
            }
        }
        textCursor.endEditBlock();
    }
}


void logic::PayloadNode::fillFromMessage(const model::Message& message)
{	
	_ui.tableWidget_path->setRowCount(0);
	_ui.tableWidget_query->setRowCount(0);

	_ui.tableWidget_path->setRowCount(50);
	_ui.tableWidget_query->setRowCount(50);

	// Disable highlighter during bulk text insertion for performance
	if (_highlighter && message.getBody().length() > 50000) {
		_highlighter->setEnabled(false);
	}

	{
		auto keys = message.getQueryParams().keys();
		for (int i = 0; i < keys.size(); i++)
		{
			_ui.tableWidget_query->setItem(i, 0, new QTableWidgetItem(keys[i]));
			_ui.tableWidget_query->setItem(i, 1, new QTableWidgetItem((message.getQueryParams()[keys[i]]).toString()));
		}
	}

	{
		auto keys = message.getPathVars().keys();
		for (int i = 0; i < keys.size(); i++)
		{
			_ui.tableWidget_path->setItem(i, 0, new QTableWidgetItem(keys[i]));
			_ui.tableWidget_path->setItem(i, 1, new QTableWidgetItem((message.getPathVars()[keys[i]]).toString()));
		}
	}

    //_ui.plainTextEdit_body->setPlainText(message.getBody());
    customDivideText(message.getBody(), _ui.plainTextEdit_body->textCursor(), 1000);
	// Re-enable highlighter after text insertion
	if (_highlighter && message.getBody().length() > 50000) {
		_highlighter->setEnabled(true);
		_highlighter->rehighlight();
	}}

void logic::PayloadNode::clearUI()
{

}

#include <QFileDialog>
#include <QGraphicsView>

void logic::PayloadNode::setupUi()
{
    //_bgColor = view::colors::byzantium;
    _bgColor = view::colors::nodes::payload;
    setSvgIcon((":/nodes/payload"));

	auto widget = new QWidget();
	_ui.setupUi(widget);

	_ui.tableWidget_path->setRowCount(50);
	_ui.tableWidget_query->setRowCount(50);

	// Set monospace font for JSON editor
	QFont font("Courier New", 10);
	font.setStyleHint(QFont::Monospace);
	_ui.plainTextEdit_body->setFont(font);

	// Enable JSON syntax highlighting
	_highlighter = new view::JSONHighlighter(_ui.plainTextEdit_body->document());

    auto payloadNode = dynamic_cast<model::PayloadNode*>(getModelNode());

    _ui.groupBox_loadFromFile->setChecked(payloadNode->getLoadFromFile());
    _ui.lineEdit_filePath->setText(payloadNode->getFilePath());

    connect(_ui.groupBox_loadFromFile, &QGroupBox::toggled, payloadNode, &model::PayloadNode::setLoadFromFile);
    connect(_ui.lineEdit_filePath, &QLineEdit::textChanged, payloadNode, &model::PayloadNode::setFilePath);

    connect(_ui.pushButton_browse, &QPushButton::clicked, this, [=]()
    {
        auto pwidget = getSceneGraph()->views().first();
        auto fileName = QFileDialog::getOpenFileName(pwidget, "Open file location", payloadNode->getFilePath(), "JSON File (*.json);; All Files (*.*);");
        if(!fileName.isEmpty())
        {
            _ui.lineEdit_filePath->setText(fileName);
            model::Message message = payloadNode->loadMessageFromFile(payloadNode->getFilePath());
            fillFromMessage(message);
        }
    });

	auto message = dynamic_cast<model::PayloadNode*>(getModelNode())->getMessage();

	fillFromMessage(message);

	getContentWidget()->layout()->addWidget(widget);

	connect(_node, &model::Node::ready, this, &PayloadNode::prepareAndSend);

	setMinSize(QSize(480, 300));
	setSize(10, 10);
}

void logic::PayloadNode::prepareAndSend()
{
    auto payloadNode = dynamic_cast<model::PayloadNode*>(getModelNode());

    if(payloadNode->getLoadFromFile())
    {
        model::Message message = payloadNode->loadMessageFromFile(payloadNode->getFilePath());
        fillFromMessage(message);
    }

    payloadNode->setMessage(composeMessage());
	_node->evaluate();
}

QMap<QString, QVariant> logic::PayloadNode::fillFromTable(QTableWidget* tableWidget) const
{
	QMap<QString, QVariant> outputMap;

	for (int i = 0; i < tableWidget->rowCount(); i++)
	{
		auto keyItem = tableWidget->item(i, 0);
		if (keyItem == nullptr) continue;
		auto key = keyItem->text();
		if (key.isEmpty()) continue;
		auto valueItem = tableWidget->item(i, 1);
		if (valueItem == nullptr) continue;
		auto value = valueItem->text();
		outputMap[key] = value;
	}

	return outputMap;
}
