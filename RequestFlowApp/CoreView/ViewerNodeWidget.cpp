#include "ViewerNodeWidget.h"
#include <QFileDialog>
#include <QGraphicsView>
#include <QAbstractItemModel>
#include <QFile>
#include <QTextStream>
#include <model/../CustomJSEngine.h>
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
    _ui.plainTextEdit_json->clear();
    _ui.treeView->reset();
    //_ui.treeWidget->clear();
    //_ui.treeView->clear();
}

void logic::ViewerNodeWidget::setupUi()
{
    auto viewerNode = getModelNode();
    _ui.setupUi(this);
    _ui.lineEdit_jsonPath->setText(viewerNode->getFilter());
    layout()->setMargin(0);

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

#include "CustomJSONModel.h"

void createItems(QString name, const QVariant& v, logic::GenericJSONItem* parent)
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


void logic::ViewerNodeWidget::setTreeModel(const QVariant& v)
{
    auto rootItem = new GenericJSONItem("root/", "root/", -1, nullptr);
    createItems("/", v, rootItem);

    auto model = _ui.treeView->model();
    if (model != nullptr)
        delete model;
    model = new CustomJSONModel(rootItem, this);
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
    //_ui.plainTextEdit_json->setPlainText(document.toJson());
    //_ui.plainTextEdit_json->clear();
    customDivideText(document.toJson(), _ui.plainTextEdit_json->textCursor(), 10000);

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
