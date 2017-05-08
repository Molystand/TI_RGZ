#include "compression_chart.h"
#include "lzw.h"
#include "qcustomplot.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <fstream>

// Декодирование
#define DECODING

Compression_chart::Compression_chart(QWidget *parent)
    : QWidget(parent), //plot(new QCustomPlot(this)),
      infile_name1(new std::string), infile_name2(new std::string), infile_name3(new std::string)
{
    // Компоновка с кнопками
    QVBoxLayout* vlout            = new QVBoxLayout();
    QPushButton* get_file1_button = new QPushButton("Выбрать файл1", this);
    QPushButton* get_file2_button = new QPushButton("Выбрать файл2", this);
    QPushButton* get_file3_button = new QPushButton("Выбрать файл3", this);
    QPushButton* coded_file       = new QPushButton("Построить диаграмму", this);
    get_file2_button->setDisabled(true);
    get_file3_button->setDisabled(true);
    coded_file->setDisabled(true);
    vlout->addWidget(get_file1_button);
    vlout->addWidget(get_file2_button);
    vlout->addWidget(get_file3_button);
    vlout->addWidget(coded_file);

    // Общая компоновка
    //QVBoxLayout* vlout = new QVBoxLayout;
    //plot = new QCustomPlot(this);     // Полотно для рисования
    //plot->setMinimumSize(200, 200);
    //vlout->addWidget(plot);
    //vlout->addLayout(hlout);


    // Сигнально слотовые соединения
    connect(get_file1_button, SIGNAL(clicked(bool)), get_file2_button, SLOT(setDisabled(bool)));
    connect(get_file2_button, SIGNAL(clicked(bool)), get_file3_button, SLOT(setDisabled(bool)));
    connect(get_file3_button, SIGNAL(clicked(bool)), coded_file,       SLOT(setDisabled(bool)));

    connect(get_file1_button, SIGNAL(clicked(bool)), this, SLOT(get_file1()));
    connect(get_file2_button, SIGNAL(clicked(bool)), this, SLOT(get_file2()));
    connect(get_file3_button, SIGNAL(clicked(bool)), this, SLOT(get_file3()));
    connect(coded_file,       SIGNAL(clicked(bool)), this, SLOT(coded()));

    // Установка компоновки, названия и размера окна
    this->setLayout(vlout);
    this->setWindowTitle("LZW");
    this->setMinimumSize(250, 100);
}

Compression_chart::~Compression_chart()
{
    //delete plot;
    delete infile_name1;
    delete infile_name2;
    delete infile_name3;
}

//----------------------------------------------------------------------------------------

void Compression_chart::get_file1()
{
    // Выбать файл из корневого каталога
    QString file_name = QFileDialog::getOpenFileName(this, "Открыть файл", QDir::homePath(), "Текстовый файл (*.txt)");
    // Выходим, если файл не был выбран
    if (file_name.length() < 1)
        return;

    *infile_name1 = file_name.toStdString();
}

void Compression_chart::get_file2()
{
    // Выбать файл из корневого каталога
    QString file_name = QFileDialog::getOpenFileName(this, "Открыть файл", QDir::homePath(), "Текстовый файл (*.txt)");
    // Выходим, если файл не был выбран
    if (file_name.length() < 1)
        return;

    *infile_name2 = file_name.toStdString();
}

void Compression_chart::get_file3()
{
    // Выбать файл из корневого каталога
    QString file_name = QFileDialog::getOpenFileName(this, "Открыть файл", QDir::homePath(), "Текстовый файл (*.txt)");
    // Выходим, если файл не был выбран
    if (file_name.length() < 1)
        return;

    *infile_name3 = file_name.toStdString();
}

//-----------------------------------------------------------------------------------

void Compression_chart::render_chart()
{
    QWidget* plot_widget = new QWidget();
    plot = new QCustomPlot(plot_widget);
    QHBoxLayout* lout = new QHBoxLayout();
    lout->addWidget(plot);
    plot_widget->setLayout(lout);


    QCPBars* bar_chart = new QCPBars(plot->xAxis, plot->yAxis);     // Гистограмма

    // Установка цвета
    QPen pen;
    pen.setWidthF(1.5);     // Толщина контура столбца
    //bar_chart->setName("Гистограмма");
    pen.setColor(QColor(70, 30, 30));      // Цвет контура столбца
    bar_chart->setPen(pen);
    bar_chart->setBrush(QColor(30, 30, 200, 100));   // Цвет столбца и прозрачность

    // Установки оси X

    QVector<double> ticks;
    //QVector<QString> labels;
    ticks << 1 << 2 << 3;// << 4 << 5 << 6 << 7;
    //labels << "1" << "2" << "3";// << "4" << "5" << "6" << "7";
//    plot->xAxis->setAutoTicks(false);
//    plot->xAxis->setAutoTickLabels(false);
//    plot->xAxis->setTickVector(ticks);
//    plot->xAxis->setTickVectorLabels(labels);
//    plot->xAxis->setSubTickCount(0);
    plot->xAxis->setTickLength(0, 4);
    //plot->xAxis->grid()->setVisible(true);
    plot->xAxis->setRange(0, 4);
    plot->xAxis->setLabel("Номер файла");

    // Установки оси Y
    plot->yAxis->setRange(0, 120);
    //plot->yAxis->setPadding(5);
    plot->yAxis->setLabel("% от исходного размера");
    //plot->yAxis->grid()->setSubGridVisible(true);
    /*QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    plot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    plot->yAxis->grid()->setSubGridPen(gridPen);*/

    // Данные
    QVector<double> data;
    data << (double)size_of_coded_file1 / size_of_file1 * 100
         << (double)size_of_coded_file2 / size_of_file2 * 100
         << (double)size_of_coded_file3 / size_of_file3 * 100;
    bar_chart->setData(ticks, data);

    // Легенда
    //plot->legend->setVisible(true);
    //plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignHCenter);
    /*plot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    plot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    plot->legend->setFont(legendFont);*/
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);     // Виды взаимодействий
    plot->replot();

    // Установка размера, названия и отрисовка
    plot_widget->setMinimumSize(400, 400);
    plot_widget->setWindowTitle("Диаграмма");
    plot_widget->show();
}

//-----------------------------------------------------------------------------------

void Compression_chart::coded()
{
    // Кодируем первый файл
    std::ifstream infile (*infile_name1);
    std::ofstream outfile("coded1.txt", std::ios::binary | std::ios::out);
    LZW_encode(infile, outfile);

    // Узнаём размеры
    infile.clear();
    infile.seekg(0, std::ios::end);
    size_of_file1 = infile.tellg();
    outfile.seekp(0, std::ios::end);
    size_of_coded_file1 = outfile.tellp();

    infile.close();
    outfile.close();
#ifdef DECODING
    infile.open("coded1.txt", std::ios::binary | std::ios::in);
    outfile.open("decoded1.txt");
    LZW_decode(infile, outfile);
    infile.close();
    outfile.close();
#endif

    // Кодируем второй файл
    infile.open(*infile_name2);
    outfile.open("coded2.txt", std::ios::binary | std::ios::out);
    LZW_encode(infile, outfile);

    // Узнаём размеры
    infile.clear();
    infile.seekg(0, std::ios::end);
    size_of_file2 = infile.tellg();
    outfile.seekp(0, std::ios::end);
    size_of_coded_file2 = outfile.tellp();

    infile.close();
    outfile.close();
#ifdef DECODING
    infile.open("coded2.txt", std::ios::binary | std::ios::in);
    outfile.open("decoded2.txt");
    LZW_decode(infile, outfile);
    infile.close();
    outfile.close();
#endif

    // Кодируем третий файл
    infile.open(*infile_name3);
    outfile.open("coded3.txt", std::ios::binary | std::ios::out);
    LZW_encode(infile, outfile);

    // Узнаём размеры
    infile.clear();
    infile.seekg(0, std::ios::end);
    size_of_file3 = infile.tellg();
    outfile.seekp(0, std::ios::end);
    size_of_coded_file3 = outfile.tellp();

    infile.close();
    outfile.close();
#ifdef DECODING
    infile.open("coded3.txt", std::ios::binary | std::ios::in);
    outfile.open("decoded3.txt");
    LZW_decode(infile, outfile);
    infile.close();
    outfile.close();
#endif

    qDebug() << size_of_file1 << " " << size_of_coded_file1;
    qDebug() << size_of_file2 << " " << size_of_coded_file2;
    qDebug() << size_of_file3 << " " << size_of_coded_file3;

    QMessageBox::information(this, "Выполнено", "Файлы успешно закодированы", QMessageBox::Ok);

    // Рисуем диаграмму
    render_chart();
}
