#include "viewernodewidget.h"
#include <QFileDialog>
#include <QGraphicsView>
#include <QAbstractItemModel>
#include <QFile>
#include <QTextStream>
#include <model/../customjsengine.h>
#include <QJsonDocument>
#include "logic/ViewerNode.h"

extern void customDivideText(const QString& data, QTextCursor textCursor, int minChunkLen);

logic::ViewerNodeWidget::ViewerNodeWidget(logic::ViewerNode* node, QWidget* parent) :QWidget(parent), _node(node)
{
    setupUi();
}

void logic::ViewerNodeWidget::clear()
{
    _ui.plainTextEdit_raw->clear();
    _ui.textEdit_json->clear();
    _ui.treeView->reset();
    //_ui.treeWidget->clear();
    //_ui.treeView->clear();
}

void logic::ViewerNodeWidget::setupUi()
{
    auto viewerNode = getModelNode();
    _ui.setupUi(this);
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

    connect(viewerNode, &model::Node::ready, this, [=]()
        {
            viewerNode->evaluate();
            auto message = viewerNode->getInputSlotsMap().first()->getData();
            setMessage(message);
            fillData();
        }, Qt::ConnectionType::QueuedConnection);

    connect(_ui.pushButton_browse, &QPushButton::clicked, this, [=]()
            {
                auto pwidget = _node->getSceneGraph()->views().first();
                auto fileName = QFileDialog::getSaveFileName(pwidget, "Save file location", viewerNode->getFilePath(), "JSON File (*.json);; All Files (*.*);");
                if(!fileName.isEmpty())
                {
                    //viewerNode->setFilePath(fileName);
                    _ui.lineEdit_filePath->setText(fileName);
                }
            });
}


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

void logic::ViewerNodeWidget::setTreeModel(const QVariant& v)
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

void logic::ViewerNodeWidget::filter(const QString& filter)
{
    model::CustomJSEngine engine;

    auto bodyValue = engine.evaluate("(" + _message.getBody() + ")");
    engine.globalObject().setProperty("_body", bodyValue);
    engine.globalObject().setProperty("_pathStr", engine.toScriptValue(filter));

    auto jsValue = engine.evaluate("pathOf(_pathStr, _body);");
    engine.globalObject().setProperty("_result", jsValue);

    auto txt = engine.fromScriptValue<QVariant>(engine.evaluate("JSON.stringify(_result);"));

    QJsonDocument document = QJsonDocument::fromJson(txt.toString().toUtf8());
    //_ui.textEdit_json->setPlainText(document.toJson());
    //_ui.textEdit_json->clear();
    customDivideText(document.toJson(), _ui.textEdit_json->textCursor(), 10000);

    auto value = engine.fromScriptValue<QVariant>(jsValue);
    setTreeModel(value);
}

void logic::ViewerNodeWidget::setMessage(model::Message message)
{
    _message = message;
}

model::Message logic::ViewerNodeWidget::getMessage() const
{
    return _message;
}

#include <math.h>

void logic::ViewerNodeWidget::fillData()
{
    auto data = _message.getBody();
    customDivideText(data, _ui.plainTextEdit_raw->textCursor(), 10000);
    //_ui.plainTextEdit_raw->setPlainText(data);
    // json
    filter(_ui.lineEdit_jsonPath->text());
}

model::ViewerNode *logic::ViewerNodeWidget::getModelNode() const
{
    return dynamic_cast<model::ViewerNode*>(_node->getModelNode());
}
