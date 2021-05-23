#include "ViewerNode.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include "Project.h"
#include "Graph.h"

model::ViewerNode::ViewerNode(model::Graph* graph) : model::Node(graph, "Viewer")
{
    setFilter("$");
    connect(this, &model::ViewerNode::evaluated, this, [=]()
    {
        if(getExportToFile())
        {
            exportToFile();
        }
    });

    setExportToFile(false);
    setFilePath(QString("./%1.json").arg(getIdentifier()));
}

void model::ViewerNode::createModel()
{
	addInputSlot("in", model::Slot::DataType::CUSTOM);
}

model::InputSlot* model::ViewerNode::getInput() const
{
	return getInputSlotsMap().values().first();
}

void model::ViewerNode::setFilter(const QString& filter)
{
	_filter = filter;
}

QString model::ViewerNode::getFilter() const
{
    return _filter;
}

void model::ViewerNode::setExportToFile(bool status)
{
    _shoudldExportToFile = status;
}

bool model::ViewerNode::getExportToFile() const
{
    return _shoudldExportToFile;
}

void model::ViewerNode::setFilePath(const QString &path)
{
    _filePath = path;
}

QString model::ViewerNode::getFilePath() const
{
    return _filePath;
}

#include <QJsonDocument>

void model::ViewerNode::exportToFile() const
{
    auto prj = getGraph()->getProject();
    auto baseDir = QDir(prj->getPath());
    auto path = baseDir.absoluteFilePath(baseDir.relativeFilePath(getFilePath()));
    QDir().mkpath(QFileInfo(path).absoluteDir().path());
    QFile fp(path);
    if(fp.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream output(&fp);
        output << QString(QJsonDocument::fromVariant(getInput()->getData().toVariant()).toJson(QJsonDocument::JsonFormat::Compact));
    }
}
