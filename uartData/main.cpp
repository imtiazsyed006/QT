#include "dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setWindowTitle("uartData");
    w.setFixedSize(840,400);
    w.show();
    return a.exec();
}
