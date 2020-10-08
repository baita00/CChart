#include "holloworld.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    holloworld w;
    w.show();

    return a.exec();
}
