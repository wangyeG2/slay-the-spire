#include "mainwindow.h"
#include <QApplication>
#include "card.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<Card>("Card");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
