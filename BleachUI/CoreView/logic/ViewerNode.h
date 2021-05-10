#pragma once

#include "../view/Node.h"
#include <QTextEdit>
#include "ui_ViewerNodeUi.h"
#include <model/ViewerNode.h>

namespace logic
{
	class AbstractItem;

	class COREVIEW_EXPORT CustomModel : public QAbstractItemModel
	{
		Q_OBJECT
	public:
		CustomModel(AbstractItem* rootItem, QObject* parent);

		virtual ~CustomModel();

		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

		QModelIndex parent(const QModelIndex& child) const override;

		int rowCount(const QModelIndex& parent = QModelIndex()) const override;

		int columnCount(const QModelIndex& parent = QModelIndex()) const override;

		QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	private:
		AbstractItem* _root = nullptr;
	};


	class ViewerNode : public view::Node
	{
		Q_OBJECT
	public:
		ViewerNode(model::ViewerNode* modelNode);
		virtual void clearUI() override;
	private:
		void setupUi();
		void setTreeModel(const QVariant& v);
		void filter(const QString& filter);
	private:
		Ui::ViewerWidgetUiForm _ui;
	};
}