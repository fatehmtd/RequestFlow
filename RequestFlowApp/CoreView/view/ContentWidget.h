#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QProgressBar>

namespace view
{
	class ContentWidget : public QWidget
	{
		Q_OBJECT
	public:
		ContentWidget(QWidget* parent);

	public slots:
		void enableProgressBar();
		void disableProgressBar();
	private:
		QProgressBar* _progressBar = nullptr;
	};
}