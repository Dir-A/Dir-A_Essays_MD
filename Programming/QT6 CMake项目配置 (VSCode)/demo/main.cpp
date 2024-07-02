#include <QApplication>
#include "mywindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWindow m;
    m.show();
    return a.exec();
}
