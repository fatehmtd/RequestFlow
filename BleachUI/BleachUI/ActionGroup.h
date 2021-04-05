#pragma once

#include <QIcon>
#include <functional>
#include <QToolButton>


namespace Ui
{
	class ActionGroupUi;
}


class ActionToolBar;
class ActionPage;

class ActionGroup : public QWidget
{
	Q_OBJECT
public:
	ActionGroup(ActionPage* page, const QString& name);
	~ActionGroup();

	QToolButton* addActionItem(const QString& name, std::function<void(void)> function, QIcon icon = QIcon());
	void removeActionItem(const QString& name);
protected:
	Ui::ActionGroupUi* _ui = nullptr;
};
