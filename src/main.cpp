#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if (argc >= 2) w.read_args(QString::fromUtf8(argv[1]));
    else w.read_args("");
    //w.show();
    w.show_player();

    return a.exec();
}
