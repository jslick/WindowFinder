#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QWidget taskbarHider;
    MainWindow* w = new MainWindow(&taskbarHider);
    w->show();

    return a.exec();
}
