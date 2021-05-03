#include "ViewerNode.h"
#include <QGraphicsProxyWidget>
#include <model/Node.h>
#include <model/Slot.h>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QAbstractItemModel>
#include <QJSValue>
#include <QJSEngine>

logic::ViewerNode::ViewerNode(model::ViewerNode* modelNode) : view::Node(modelNode, "Viewer")
{
	setupUi();
}

void logic::ViewerNode::clearUI()
{
	_ui.plainTextEdit_raw->clear();
	_ui.textEdit_json->clear();
	_ui.treeWidget->clear();
}

#include <QDebug>

void logic::ViewerNode::setupUi()
{
	auto viewerNode = dynamic_cast<model::ViewerNode*>(getModelNode());

	auto widget = new QWidget();
	_ui.setupUi(widget);
	_ui.lineEdit_jsonPath->setText(viewerNode->getFilter());

	connect(_ui.lineEdit_jsonPath, &QLineEdit::textChanged, viewerNode, &model::ViewerNode::setFilter);

	connect(_ui.toolButton_filter, &QToolButton::clicked, this, [=]() 
		{
			filter(_ui.lineEdit_jsonPath->text());
		});

	connect(_ui.lineEdit_jsonPath, &QLineEdit::returnPressed, this, [=]()
		{
			filter(_ui.lineEdit_jsonPath->text());
		});
		
	getContentWidget()->layout()->addWidget(widget);
	_bgColor = view::colors::green;
	connect(_node, &model::Node::ready, this, [=]()
		{
			_node->evaluate();

			auto message = _node->getInputSlotsMap().first()->getData();
			auto data = message.getBody();

			_ui.plainTextEdit_raw->setPlainText(data);

			// json
			filter(_ui.lineEdit_jsonPath->text());

		}, Qt::ConnectionType::QueuedConnection);

	setMinSize(QSize(400, 300));
	setSize(300, 200);
}

#include <QTreeWidgetItem>
#include <QFont>

QTreeWidgetItem* toWidgetItem(const QString& name, const QVariant& v, QTreeWidgetItem* parent)
{
	if (!v.isValid()) return nullptr;

	auto widgetItem = new QTreeWidgetItem(parent);

	widgetItem->setData(0, Qt::DisplayRole, name);
	widgetItem->setData(0, Qt::ForegroundRole, QColor("#0056A0"));


	switch (v.type())
	{
	case QVariant::Type::Map:
	{
		auto variantMap = v.value<QVariantMap>();
		widgetItem->setData(1, Qt::DisplayRole, QString("Object {%1}").arg(variantMap.size()));
		widgetItem->setData(0, Qt::DecorationRole, QIcon(":/ui/{}"));
		widgetItem->setData(0, Qt::ForegroundRole, QColor(0, 0, 255));
		widgetItem->setData(1, Qt::ForegroundRole, QColor(0, 0, 180));
		QFont font;
		font.setBold(true);
		widgetItem->setData(0, Qt::FontRole, font);
		widgetItem->setData(1, Qt::FontRole, font);
		for (const auto& key : variantMap.keys())
		{
			toWidgetItem(key, variantMap[key], widgetItem);
		}
	}
	break;
	case QVariant::Type::List:
	{
		auto list = v.value<QVariantList>();
		widgetItem->setData(1, Qt::DisplayRole, QString("Array [%1]").arg(list.size()));
		widgetItem->setData(0, Qt::DecorationRole, QIcon(":/ui/[]"));
		widgetItem->setData(0, Qt::ForegroundRole, QColor(0, 0, 180));
		widgetItem->setData(1, Qt::ForegroundRole, QColor(0, 0, 180));
		QFont font;
		font.setBold(true);
		widgetItem->setData(0, Qt::FontRole, font);
		widgetItem->setData(1, Qt::FontRole, font);
		int index = 0;
		for (const auto& value : list)
		{
			toWidgetItem(QString("[%1]").arg(index++), value, widgetItem);
		}
	}
	break;
	case QVariant::Type::String:
		widgetItem->setData(1, Qt::ForegroundRole, QColor("#AC0D8A"));
		widgetItem->setData(1, Qt::DisplayRole, v.toString());
		break;
	case QVariant::Type::Bool:
		widgetItem->setData(1, Qt::ForegroundRole, QColor("#002CF5"));
		widgetItem->setData(1, Qt::DisplayRole, v.toString());
		break;
	case QVariant::Type::Int:
	case QVariant::Type::UInt:
	case QVariant::Type::LongLong:
	case QVariant::Type::ULongLong:
	case QVariant::Type::Double:
		widgetItem->setData(1, Qt::ForegroundRole, QColor("#00875C"));
		widgetItem->setData(1, Qt::DisplayRole, v.toString());
		break;
	default:
		widgetItem->setData(1, Qt::DisplayRole, v.toString());
		break;
	}

	return widgetItem;
}

void logic::ViewerNode::setJSValue(const QVariant& v)
{
	_ui.treeWidget->clear();
	auto item = toWidgetItem("", v, nullptr);
	_ui.treeWidget->insertTopLevelItem(0, item);
	auto index = _ui.treeWidget->currentIndex();

	_ui.treeWidget->expandAll();
}

#include <QFile>
#include <QTextStream>

void logic::ViewerNode::filter(const QString& filter)
{
	QJSEngine engine;
	engine.installExtensions(QJSEngine::Extension::AllExtensions);

	{
		QFile fp(":/js/jsonpath");
		if (fp.open(QIODevice::ReadOnly))
		{
			QTextStream data(&fp);
			auto jsonPath = engine.evaluate(data.readAll());
			auto pathOfFunction = engine.evaluate("(function (path, obj) { return JSONPath.JSONPath(path, obj);})");
			engine.globalObject().setProperty("pathOf", pathOfFunction);
		}
	}

	auto message = _node->getInputSlotsMap().first()->getData(); 
	auto bodyValue = engine.evaluate("(" + message.getBody() + ")");
	engine.globalObject().setProperty("_body", bodyValue);
	engine.globalObject().setProperty("_pathStr", engine.toScriptValue(filter));

	auto jsValue = engine.evaluate("pathOf(_pathStr, _body);");
	engine.globalObject().setProperty("_result", jsValue);

	auto txt = engine.fromScriptValue<QVariant>(engine.evaluate("JSON.stringify(_result);"));

	QJsonDocument document = QJsonDocument::fromJson(txt.toString().toUtf8());
	_ui.textEdit_json->setPlainText(document.toJson());

	auto value = engine.fromScriptValue<QVariant>(jsValue);
	setJSValue(value);
}
