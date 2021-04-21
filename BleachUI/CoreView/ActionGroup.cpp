#include "ActionGroup.h"
#include "ActionToolBar.h"
#include "ActionPage.h"
#include "ui_ActionGroup.h"

#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QDebug>

ActionGroup::ActionGroup(ActionPage* page, const QString& name) : QWidget(page), _ui(new Ui::ActionGroupUi)
{
	setObjectName(name);
	_ui->setupUi(this);
	_ui->label->setText(name);
}

ActionGroup::~ActionGroup()
{
	delete _ui;
}

QToolButton* ActionGroup::addActionItem(const QString& name, std::function<void(void)> function, QIcon icon)
{
	auto button = new QToolButton();
	button->setText(name);
	button->setObjectName(name);
	button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	button->setMinimumSize(48, 48);
	button->setIconSize(QSize(32, 32));
	button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	button->setAutoRaise(true);
	button->setIcon(icon);

	connect(button, &QToolButton::clicked, function);

	_ui->horizontalLayout->addWidget(button);
	//button->setPopupMode(QToolButton::ToolButtonPopupMode::MenuButtonPopup);
	return button;
}

void ActionGroup::removeActionItem(const QString& name)
{
	auto button = findChild<QToolButton*>(name);
	if (button != nullptr) delete button;
}
