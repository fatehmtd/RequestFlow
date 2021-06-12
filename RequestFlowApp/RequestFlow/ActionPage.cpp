#include "ActionPage.h"
#include "ActionToolBar.h"
#include "ActionGroup.h"
#include "ui_ActionPage.h"
#include <QPushButton>
#include <QDebug>

ActionPage::ActionPage(ActionToolBar* toolBar, const QString& name) : QWidget(toolBar->getWidget()), _ui(new Ui::ActionPageUi)
{
    setObjectName(name);
    _ui->setupUi(this);
}


ActionPage::~ActionPage()
{
    delete _ui;
}

ActionGroup* ActionPage::addGroup(const QString& title)
{
    auto group = getGroup(title);
    if (group) return group;
    group = new ActionGroup(this, title);
    _ui->horizontalLayout->addWidget(group);

    return group;
}

ActionGroup* ActionPage::getGroup(const QString& title, bool createIfInexistant)
{
    auto group = findChild<ActionGroup*>(title);
    if (group) return group;

    if(!createIfInexistant) return nullptr;

    group = new ActionGroup(this, title);
    _ui->horizontalLayout->addWidget(group);

    return group;
}

void ActionPage::removeGroup(const QString& title)
{
    auto group = getGroup(title);
    if (group != nullptr) delete group;
}
