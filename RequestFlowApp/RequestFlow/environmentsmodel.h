#pragma once

#include <QAbstractItemModel>
#include <QObject>
#include <QModelIndex>
#include <QIcon>

#include <model/Project.h>
#include <model/Environment.h>


class EnvironmentsModel : public QAbstractItemModel
{
public:
    EnvironmentsModel(QObject* parent);

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void setProject(model::Project* project);

    void beginResetModel();

    void endResetModel();
private:
    model::Project* _project = nullptr;
    QIcon _envIcon;
};
