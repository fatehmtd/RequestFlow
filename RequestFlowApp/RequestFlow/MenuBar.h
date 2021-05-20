#pragma once

#include "MainWindow.h"

class MenuBar : public QObject
{
    Q_OBJECT
public:
    MenuBar(QMainWindow* mainWindow);
private:
    QMainWindow* _parent= nullptr;
};
