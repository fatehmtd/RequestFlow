#pragma once
#include <QWidget>
#include <QVBoxLayout>

namespace view
{
	class ContentWidget : public QWidget
	{
		Q_OBJECT
	public:
		ContentWidget(QWidget* parent);

	protected:
		QVBoxLayout* _layout = nullptr;
	};
}

