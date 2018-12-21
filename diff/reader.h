#ifndef READER_H
#define READER_H

#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <experimental/filesystem>
#include <QApplication>
#include <QProgressBar>

namespace fs = std::experimental::filesystem;

class bits {
public:
    bits();
    bits(char x, char y, char z);

    friend bool operator==(bits const& a, bits const& b);
    friend bool operator<(bits const& a, bits const& b);

    char x, y, z;
};


std::map<std::pair<size_t, bits>, std::vector<fs::path>> get(std::string directory, QProgressBar *bar, int mode);

QStringList read(std::string directory, QProgressBar *bar, int mode);

QStringList read(QString directory, QProgressBar *bar, int mode);

QStringList read_update(std::string directory, QProgressBar *bar, int mode);

QStringList get_same(std::string name, int mode);







#endif // READER_H
