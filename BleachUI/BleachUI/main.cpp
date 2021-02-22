#include "BleachUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BleachUI w;
    w.show();

    return a.exec();
}

