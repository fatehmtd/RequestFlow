#pragma once

#include <QObject>
#include <QIcon>

class InventoryItem : public QObject
{
    Q_OBJECT
public:
    enum Type
    {
        Root,
        Document,
        Endpoint,
        Node
    };

    InventoryItem(int type=Root, InventoryItem* parent=nullptr);

    InventoryItem *getParent() const;

    QList<InventoryItem*> getChildren() const;

    void setData(const QString& data);
    QString getData() const;

    int getType() const;
    int isDraggable() const;
    bool isEditable() const;

    void setIcon(QIcon icon);
    QIcon getIcon() const;

    void setUserDataPtr(void* ptr);
    void *getUserDataPtr() const;

    int getColCount() const;
    void setColCount(int c);

    template<typename T>
    T getUserDataPtr() const
    {
        return reinterpret_cast<T>(getUserDataPtr());
    }
private:
    int _colCount = 1;
    int _type;
    QIcon _icon;
    QString _data;
    void *_dataPtr = nullptr;
};
