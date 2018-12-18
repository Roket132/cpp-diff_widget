#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <fstream>
#include <string>
#include <experimental/filesystem>
#include <sha256.h>

namespace fs = std::experimental::filesystem;
using namespace std;

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QStringList List;
    List << "1" << "two" << "there/ fwe ";
    w.add_Items_Main(List);

    return a.exec();
}
