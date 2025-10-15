#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Pixify");
    w.setWindowIcon(QIcon(":/icons/icons/pixify.png"));
    w.show();
    return a.exec();
}
