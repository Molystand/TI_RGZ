#include "compression_chart.h"
#include "lzw.h"
//#include "qcustomplot.h"
#include <QtWidgets>
#include <fstream>

// Декодирование
#define DECODING

Compression_chart::Compression_chart(QWidget *parent)
    : QWidget(parent),
      infile_name1(new std::string), infile_name2(new std::string), infile_name3(new std::string)
{
    QHBoxLayout* hlout            = new QHBoxLayout;
    QPushButton* get_file1_button = new QPushButton("Выбрать файл1", this);
    QPushButton* get_file2_button = new QPushButton("Выбрать файл2", this);
    QPushButton* get_file3_button = new QPushButton("Выбрать файл3", this);
    QPushButton* coded_file       = new QPushButton("Построить диаграмму", this);
    get_file2_button->setDisabled(true);
    get_file3_button->setDisabled(true);
    coded_file->setDisabled(true);
    hlout->addWidget(get_file1_button);
    hlout->addWidget(get_file2_button);
    hlout->addWidget(get_file3_button);
    hlout->addWidget(coded_file);

    QVBoxLayout* vlout = new QVBoxLayout;
    //plot = new QCustomPlot;     // Полотно для рисования
    //vlout->addWidget(plot, 10);
    vlout->addLayout(hlout, 1);


    // Сигнально слотовые соединения
    connect(get_file1_button, SIGNAL(clicked(bool)), get_file2_button, SLOT(setDisabled(bool)));
    connect(get_file2_button, SIGNAL(clicked(bool)), get_file3_button, SLOT(setDisabled(bool)));
    connect(get_file3_button, SIGNAL(clicked(bool)), coded_file,       SLOT(setDisabled(bool)));

    connect(get_file1_button, SIGNAL(clicked(bool)), this, SLOT(get_file1()));
    connect(get_file2_button, SIGNAL(clicked(bool)), this, SLOT(get_file2()));
    connect(get_file3_button, SIGNAL(clicked(bool)), this, SLOT(get_file3()));
    connect(coded_file,       SIGNAL(clicked(bool)), this, SLOT(coded()));

    // Установка компоновки и размера окна
    this->setLayout(vlout);
    this->setMinimumSize(420, 400);
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

//---------------------------------------------------------------------

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
}

