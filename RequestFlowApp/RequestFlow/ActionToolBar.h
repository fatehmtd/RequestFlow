#pragma once

#include <QTabWidget>
#include <QMap>
#include <memory>
#include <QIcon>
#include "ActionGroup.h"

class ActionGroup;
class ActionPage;

class ActionToolBar : public QObject
{
	Q_OBJECT
public:
	ActionToolBar(QTabWidget* tabWidget);
	~ActionToolBar();

	QTabWidget* getWidget() const;

	ActionPage* addPage(const QString& name, QIcon icon = QIcon());
	ActionPage* getPage(const QString& name);
	void removePage(const QString& name);
protected:
	void init();
protected:
	QTabWidget* _tabWidget = nullptr;
};
