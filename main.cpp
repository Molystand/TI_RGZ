#include "compression_chart.h"
#include <QApplication>
#include <iostream>
#include <fstream>
#include "lzw.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Compression_chart w;
    w.show();

    return a.exec();

    /*
    std::ifstream infile ("message.txt");
    std::ofstream outfile("coded.txt");

    LZW_encode(infile,  outfile);

    infile.close();
    outfile.close();
    std::ifstream infile1 ("coded.txt");
    std::ofstream outfile1("decoded.txt");

    LZW_decode(infile1, outfile1);
    */
}
