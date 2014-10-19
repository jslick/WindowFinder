#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    QWidget taskbarHider;
    MainWindow* w = new MainWindow(&taskbarHider);
    QObject::connect(w, SIGNAL(closed(QCloseEvent*)), &a, SLOT(quit()));
    w->show();

    return a.exec();
}
