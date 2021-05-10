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

#if QT_VERSION >= QT_VERSION_CHECK(5,6,0)
    QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
#else
    qputenv("QT_DEVICE_PIXEL_RATIO", QByteArray("1"));
#endif // QT_VERSION
}

int main(int argc, char *argv[])
{
    enableHDPISupport();    
    QApplication a(argc, argv);

#ifndef _DEBUG
    auto choice = QMessageBox::warning(nullptr, "Conditions d'utilisation de RequestFlow", "RequestFlow est la propriete intelectuelle de Fateh Benmerzoug PhD (fatehmtd@gmail.com).\nL'usage ou la distribution de cet outil sans l'accord prealable de l'auteur est strictement interdite.\nMerci de supprimer RequestFlow de votre machine si vous n'etes pas d'accord avec les conditions d'utilisation.\nAccepter ces conditions?", QMessageBox::Yes, QMessageBox::No);
    if (choice != QMessageBox::Yes) return 0;
#endif

    MainWindow w;
    w.showMaximized();
    return a.exec();
}

