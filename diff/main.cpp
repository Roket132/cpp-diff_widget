#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <fstream>
#include <string>
#include <experimental/filesystem>
#include <sha256.h>

namespace fs = std::experimental::filesystem;



using namespace std;

int main()
{
    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();
    */

    string input = "grape";
    string output1 = sha256(input);

    fs::path path = "/home/dmitry/QtProject/diff_widget/build-diff-Desktop_Qt_5_12_0_GCC_64bit-Debug/txt";

    std::ifstream in(path, std::ios::in);


    cout << "sha256 = " << sha256_file(path) << endl;


    //return a.exec();
}
