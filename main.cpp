#include "mainwindow.h"
#include <QApplication>
#include <QTime>
#include <QtGlobal>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(uint(QTime::currentTime().msec()));
    MainWindow w;
    w.show();

    return a.exec();
}
