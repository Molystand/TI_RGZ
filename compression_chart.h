// compression_chart.h
#ifndef COMPRESSION_CHART_H
#define COMPRESSION_CHART_H

#include <QWidget>
#include <string>
#include "lzw.h"

class QCustomPlot;


class Compression_chart : public QWidget
{
    Q_OBJECT

public:
    Compression_chart(QWidget *parent = 0);
    ~Compression_chart();

public slots:
    void get_file1();
    void get_file2();
    void get_file3();
    void coded();

private:
    std::string* infile_name1;
    std::string* infile_name2;
    std::string* infile_name3;

    long long size_of_file1;
    long long size_of_file2;
    long long size_of_file3;

    long long size_of_coded_file1;
    long long size_of_coded_file2;
    long long size_of_coded_file3;

    QCustomPlot* plot;
    void render_chart();
};

#endif // COMPRESSION_CHART_H
