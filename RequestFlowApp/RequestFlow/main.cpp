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

void enableDarkTheme()
{
    //QFile f(":qdarkstyle/theme/style.qss");
    QFile f(":/qdarkstyle/dark/style.qss");

    if (!f.exists())   {
        qDebug() << "Unable to set stylesheet, file not found\n";
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // TODO: fix the crash caused by the dark theme
    //enableDarkTheme();

    MainWindow w;
    w.show();

#ifndef _DEBUG
    //if(!showLicense(&w)) return 0;
#endif

    return a.exec();
}

