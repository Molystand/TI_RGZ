#include "compression_chart.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Compression_chart w;
    w.show();

    return a.exec();
}
