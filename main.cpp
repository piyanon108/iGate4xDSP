#include <QCoreApplication>
#include "iGate4x.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    iGate4x igate3x;
    return a.exec();
}
