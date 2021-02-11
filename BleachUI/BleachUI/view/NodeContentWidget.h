#pragma once

#include <QWidget>
namespace Ui { class NodeContentWidget; };

namespace view
{
	class NodeContentWidget : public QWidget
	{
		Q_OBJECT

	public:
		NodeContentWidget(QWidget* parent = Q_NULLPTR);
		~NodeContentWidget();

	private:
		Ui::NodeContentWidget* ui;
	};
}
