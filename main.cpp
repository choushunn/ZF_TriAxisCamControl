#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.setStyle(QStyleFactory::create("Fusion"));
    a.setWindowIcon(QIcon(":/img/favicon.ico"));
    w.show();
    return a.exec();
}
