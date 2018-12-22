#include "mainwindow.h"
#include "reader.h"

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
    std::cout << "1" << std::endl;
    w.show();



    return a.exec();
}
