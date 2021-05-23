#pragma once
#include "../coremodel_global.h"
#include "Node.h"
#include "Message.h"
#include "Graph.h"
#include "Slot.h"

namespace model
{
    class COREMODEL_EXPORT PayloadNode : public model::Node
    {
        Q_OBJECT
        Q_PROPERTY(QVariant message READ messageToVariant WRITE messageFromVariant)
        Q_PROPERTY(QString filePath MEMBER _filePath READ getFilePath WRITE setFilePath)
        Q_PROPERTY(bool loadFromFileStatus MEMBER _loadFromFileStatus READ getLoadFromFile WRITE setLoadFromFile)
    public:
        Q_INVOKABLE PayloadNode(model::Graph* graph);
        void createModel() override;


        model::Message getMessage() const;

        void evaluate() override;
        OutputSlot* getOutputSlot() const;

        bool getLoadFromFile() const;
        QString getFilePath() const;

        model::Message loadMessageFromFile(const QString& path) const;
    public slots:
        void setLoadFromFile(bool status);
        void setMessage(const model::Message& message);
        void setFilePath(const QString& path);

    protected:
        QVariant messageToVariant() const;
        void messageFromVariant(const QVariant& v);
    private:
        model::Message _message;
        bool _loadFromFileStatus = false;
        QString _filePath;
    };
}
