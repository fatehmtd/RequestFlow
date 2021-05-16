#pragma once
#include "../coremodel_global.h"
#include "Node.h"
#include "Message.h"
#include "Graph.h"
#include "Slot.h"

namespace model
{
    class Graph;

    class COREMODEL_EXPORT ViewerNode : public model::Node
    {
        Q_OBJECT
        Q_PROPERTY(QString filter MEMBER _filter READ getFilter WRITE setFilter)
        Q_PROPERTY(bool exportToFileStatus MEMBER _shoudldExportToFile READ getExportToFile WRITE setExportToFile)
        Q_PROPERTY(QString filePath MEMBER _filePath READ getFilePath WRITE setFilePath)
    public:
        Q_INVOKABLE ViewerNode(model::Graph* graph);

        void createModel() override;

        model::InputSlot* getInput() const;

        QString getFilter() const;        
        bool getExportToFile() const;        
        QString getFilePath() const;

    public slots:
        void setFilePath(const QString& path);
        void setExportToFile(bool status);
        void setFilter(const QString& filter);

    private:
        void exportToFile() const;
    private:
        QString _filter;
        QString _filePath;
        bool _shoudldExportToFile;
    };
}
