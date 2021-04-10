#pragma once

#include <QWidget>
#include <QHBoxLayout>



namespace Ui
{
	class ActionPageUi;
}


class ActionToolBar;
class ActionGroup;

class ActionPage : public QWidget
{
	Q_OBJECT
public:
	ActionPage(ActionToolBar* toolBar, const QString& name);
	~ActionPage();

	ActionGroup* addGroup(const QString& title);
	ActionGroup* getGroup(const QString& title, bool createIfInexistant = true);
	void removeGroup(const QString& title);
protected:
	Ui::ActionPageUi* _ui = nullptr;
};
