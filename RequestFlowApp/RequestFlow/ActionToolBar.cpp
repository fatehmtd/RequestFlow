#include "ActionToolBar.h"
#include "ActionPage.h"
#include <QDebug>
#include <QApplication>


ActionToolBar::ActionToolBar(QTabWidget* widget) : _tabWidget(widget)
{
	init();
}

ActionToolBar::~ActionToolBar()
{
}

QTabWidget* ActionToolBar::getWidget() const
{
	return _tabWidget;
}

ActionPage* ActionToolBar::addPage(const QString& name, QIcon icon)
{
	// check if existing
	auto actionPage = _tabWidget->findChild<ActionPage*>(name);
	if (actionPage != nullptr) return actionPage;

	// create and return
	actionPage = new ActionPage(this, name);
	_tabWidget->addTab(actionPage, icon, name);
	return actionPage;
}

ActionPage* ActionToolBar::getPage(const QString& name)
{
	auto actionPage = _tabWidget->findChild<ActionPage*>(name);
	if (actionPage != nullptr) return actionPage;
	//THROW_EXCEPTION("ActiontoolBar", QString("Could not find the requested page : %1").arg(name));
	return nullptr;
}

void ActionToolBar::removePage(const QString& name)
{
	// check if existing
	auto actionPage = _tabWidget->findChild<ActionPage*>(name);
	if (actionPage != nullptr) delete actionPage;
}

void ActionToolBar::init()
{
	// Determine default colors
    //QColor bg = qApp->palette().color(QPalette::Background);
    QColor bg = qApp->palette().color(QPalette::Base);
	QColor mid = qApp->palette().color(QPalette::Mid); 

	QString styleSheetText = QString(
		"QTabBar::tab {width: 80px; height: 34px;}"
	);

	//_tabWidget->setStyleSheet(styleSheetText);
	_tabWidget->setAutoFillBackground(true);

	// Note: the order in which the background/palette/stylesheet functions are
	// called does matter. Should be same as in Qt designer.
	//_tabWidget->setAutoFillBackground(true);

	/*
	// Set stylesheet
	QString styleSheetText = QString(
		"QTabWidget::pane {"
		"  border-top: 1px solid rgb(%4, %5, %6);"
		"  position: absolute;"
		"  top: -1px;"
		"}"
		""
		"QTabBar::tab {"
		"  padding-top: 5px;"
		"  padding-bottom: 5px;"
		"  padding-left: 10px;"
		"  padding-right: 10px;"
		"  margin-top: 1px;"
		"}"
		""
		"QTabBar::tab::!selected {"
		"  border-bottom: 1px solid rgb(%4, %5, %6);"
		"  background-color:#ffffff;"
		"}"
		""
		"QTabBar::tab:selected {"
		"  background-color: rgb(%1, %2, %3);"
		"  border-top: 1px solid rgb(%4, %5, %6);"
		"  border-right: 1px solid rgb(%4, %5, %6);"
		"  border-left: 1px solid rgb(%4, %5, %6);"
		"  border-bottom: 1px solid rgb(%1, %2, %3);"
		"}"
		""
		"QTabBar::tab:hover"
		"{"
		"  background-color: rgb(205, 232, 255);"
		"}"
		""
		"QTabBar::tab:selected:hover {"
		"  background-color: rgb(%1, %2, %3);"
		"}"
	).arg(bg.red()).arg(bg.green()).arg(bg.blue())
		.arg(mid.red()).arg(mid.green()).arg(mid.blue());

	//_tabWidget->setStyleSheet(styleSheetText);

	// Set background color
	QPalette pal = _tabWidget->palette();
	pal.setColor(QPalette::Background, Qt::white);
	//_tabWidget->setPalette(pal);
	//*/
}
