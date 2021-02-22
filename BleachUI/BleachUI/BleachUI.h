#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BleachUI.h"
#include "./view/SceneGraphWidget.h"

class BleachUI : public QMainWindow
{
    Q_OBJECT

public:
    BleachUI(QWidget *parent = Q_NULLPTR);

private:
    Ui::BleachUIClass ui;
    SceneGraphWidget* _sceneGraphWidget = nullptr;
};
