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
#include <QFileDialog>
#include <QGraphicsView>

logic::ViewerNode::ViewerNode(model::ViewerNode* modelNode) : view::Node(modelNode, "Viewer")
{
	setupUi();
}

void logic::ViewerNode::clearUI()
{
	_ui.plainTextEdit_raw->clear();
	_ui.textEdit_json->clear();
	_ui.treeView->reset();
	//_ui.treeWidget->clear();
	//_ui.treeView->clear();
}

#include <QDebug>

void logic::ViewerNode::setupUi()
{
	auto viewerNode = dynamic_cast<model::ViewerNode*>(getModelNode());

	auto widget = new QWidget();
	_ui.setupUi(widget);
	_ui.lineEdit_jsonPath->setText(viewerNode->getFilter());

    _ui.groupBox->setChecked(viewerNode->getExportToFile());
    _ui.lineEdit_filePath->setText(viewerNode->getFilePath());

    connect(_ui.groupBox, &QGroupBox::toggled, viewerNode, &model::ViewerNode::setExportToFile);
    connect(_ui.lineEdit_filePath, &QLineEdit::textChanged, viewerNode, &model::ViewerNode::setFilePath);

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

    connect(_ui.pushButton_browse, &QPushButton::clicked, this, [=]()
    {
        auto pwidget = getSceneGraph()->views().first();
        auto fileName = QFileDialog::getSaveFileName(pwidget, "Save file location", viewerNode->getFilePath(), "JSON File (*.json);; All Files (*.*);");
        if(!fileName.isEmpty())
        {
            //viewerNode->setFilePath(fileName);
            _ui.lineEdit_filePath->setText(fileName);
        }
    });

	setMinSize(QSize(400, 300));
	setSize(300, 200);
}

#include <QAbstractItemModel>

namespace logic
{
	class AbstractItem
	{
	public:
		AbstractItem(const QString& path, const QString& value, int type, AbstractItem* parent)
			: _path(path), _value(value), _type(type), _parent(parent)
		{
			switch (type)
			{
			case QVariant::Type::Map:
			{
				_decoration = QIcon(":/ui/{}");
				_foreground = QColor(0, 0, 255);
				break;
			}
			case QVariant::Type::List:
			{
				_decoration = QIcon(":/ui/[]");
				_foreground = QColor(0, 0, 255);
				break;
			}
			case QVariant::Type::String:
			{
				_foreground = QColor("#AC0D8A");
				break;
			}
			case QVariant::Type::Bool:
			{
				_foreground = QColor("#002CF5");
				break;
			}
			case QVariant::Type::Int:
			case QVariant::Type::UInt:
			case QVariant::Type::LongLong:
			case QVariant::Type::ULongLong:
			case QVariant::Type::Double:
			{
				_foreground = QColor("#00875C");
				break;
			}
			}
		}

		~AbstractItem()
		{
			qDeleteAll(_children);
		}

		AbstractItem* getParent() const
		{
			return _parent;
		}

		QList<AbstractItem*> getChildren() const
		{
			return _children;
		}

		void setPath(const QString& path)
		{
			_path = path;
		}

		QString getPath() const { return _path; }

		void setValue(const QString& value)
		{
			_value = value;
		}

		QString getValue() const { return _value; }

		int row() const
		{
			if (_parent != nullptr)
			{
				return _parent->_children.indexOf(const_cast<AbstractItem*>(this));
			}
			return 0;
		}

		AbstractItem* addChild(const QString& path, const QString& value, int type)
		{
			auto item = new AbstractItem(path, value, type, this);
			_children << item;
			++_numChildren;
			return item;
		}

		void setModelIndex(QModelIndex index)
		{
			_modelIndex = index;
		}

		QModelIndex getModelIndex() const
		{
			return _modelIndex;
		}

		int getNumChildren() const
		{
			return _numChildren;
		}

		int getType() const
		{
			return _type;
		}

		QVariant getForeground() const {
			return _foreground;
		}
		
		QVariant getDecoration() const {
			return _decoration;
		}
	private:
		QString _path, _value;
		int _type;
		AbstractItem* _parent = nullptr;
		QList<AbstractItem*> _children;

		QVariant _foreground, _decoration;

		QModelIndex _modelIndex;
		int _numChildren = 0;
	};

	void createItems(QString name, const QVariant& v, AbstractItem* parent)
	{
		auto item = parent->addChild(name, "", v.type());

		switch (v.type())
		{
		case QVariant::Type::Map:
		{
			auto variantMap = v.value<QVariantMap>();
			item->setValue(QString("Object {%1}").arg(variantMap.size()));

			for (const auto& key : variantMap.keys())
			{
				createItems(key, variantMap[key], item);
			}
		}
		break;
		case QVariant::Type::List:
		{
			auto list = v.value<QVariantList>();
			item->setValue(QString("Array [%1]").arg(list.size()));
			int index = 0;
			for (const auto& value : list)
			{
				createItems(QString("[%1]").arg(index++), value, item);
			}
		}
		break;
		default:
			item->setValue(v.toString());
			break;
		}
	}
}

logic::CustomModel::CustomModel(AbstractItem* rootItem, QObject* parent) : _root(rootItem), QAbstractItemModel(parent)
{

}

logic::CustomModel::~CustomModel()
{
	if (_root != nullptr)
		delete _root;
}

QModelIndex logic::CustomModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	AbstractItem* parentItem = nullptr;

	if (!parent.isValid())
		parentItem = _root;
	else	
		parentItem = static_cast<AbstractItem*>(parent.internalPointer());

	auto item = parentItem->getChildren()[row];

	auto modelIndex = createIndex(row, column, item);
	return modelIndex;
}
  
QModelIndex logic::CustomModel::parent(const QModelIndex& child) const
{
	if (!child.isValid())
		return QModelIndex();

	auto childItem = static_cast<AbstractItem*>(child.internalPointer());

	auto parentItem = childItem->getParent();
	
	if (parentItem == nullptr)
		return QModelIndex();

	auto modelIndex = createIndex(parentItem->row(), 0, parentItem);
	return modelIndex;
}

int logic::CustomModel::rowCount(const QModelIndex& parent) const
{
	AbstractItem* parentItem = nullptr;

	if (!parent.isValid())
		parentItem = _root;
	else
		parentItem = static_cast<AbstractItem*>(parent.internalPointer());

	return parentItem->getNumChildren();
}

int logic::CustomModel::columnCount(const QModelIndex& parent) const
{
	return 2;
}

QVariant logic::CustomModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (section == 0) return "Key";
		else return "Value";
	}
	return QAbstractItemModel::headerData(section, orientation, role);
}

QVariant logic::CustomModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid()) return QVariant();

	auto item = static_cast<AbstractItem*>(index.internalPointer());

	switch (role)
	{
	case Qt::DisplayRole:
	case Qt::EditRole:
	{
		switch (index.column())
		{
		case 0:
			return item->getPath();
		case 1:
			return item->getValue();
		default:
			return QVariant();
		}
	}
	case Qt::ForegroundRole:
	{
		return item->getForeground();
	}
	case Qt::DecorationRole:
	{
		return item->getDecoration();
	}
	}
	return QVariant();
}

void logic::ViewerNode::setTreeModel(const QVariant& v)
{
	auto rootItem = new AbstractItem("root/", "root/", -1, nullptr);
	createItems("/", v, rootItem);

	auto model = _ui.treeView->model();
	if (model != nullptr)
		delete model;
	model = new CustomModel(rootItem, this);
	model->setHeaderData(0, Qt::Orientation::Horizontal, "Key");
	model->setHeaderData(1, Qt::Orientation::Horizontal, "Value");
	_ui.treeView->setModel(model);
	_ui.treeView->update();
	_ui.treeView->expand(model->index(0, 0));
	_ui.treeView->setHeaderHidden(false);
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
	setTreeModel(value);
}
