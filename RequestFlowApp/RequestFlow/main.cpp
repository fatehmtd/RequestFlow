#include "MainWindow.h"
#include <QtWidgets/QApplication>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include <QMessageBox>
#include <QInputDialog>

void enableHDPISupport()
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#ifdef Q_OS_WIN
    ::SetProcessDPIAware(); // call before the main event loop
#endif // Q_OS_WIN
/*
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0)
    QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
#else
    qputenv("QT_DEVICE_PIXEL_RATIO", QByteArray("1"));
#endif // QT_VERSION
    //*/
}

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
    enableHDPISupport();    
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

#ifndef _DEBUG
    //if(!showLicense(&w)) return 0;
#endif

    return a.exec();
}

