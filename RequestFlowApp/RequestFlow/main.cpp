#include "MainWindow.h"
#include <QtWidgets/QApplication>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include <QMessageBox>
#include <QInputDialog>

#include <QFile>
#include <QTextStream>

bool showLicense(QWidget* parent)
{
    QFile fp(":/misc/license.txt");
    if(!fp.open(QIODevice::ReadOnly)) return false;
    QString content = QString(fp.readAll());

    auto choice = QMessageBox::warning(parent, "Conditions d'utilisation de RequestFlow", content, QMessageBox::Yes, QMessageBox::No);
    return choice == QMessageBox::Yes;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

#ifndef _DEBUG
    //if(!showLicense(&w)) return 0;
#endif

    return a.exec();
}

