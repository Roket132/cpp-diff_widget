#include "reader.h"
#include "mainwindow.h"
#include "file_data.h"
#include "sha256.h"

bits::bits() :x(0), y(0), z(0) {}

bits::bits(char x, char y, char z) : x(x), y(y), z(z) {}

bool operator==(bits const& a, bits const& b) {
    return (a.x == b.x);
}
bool operator<(bits const& a, bits const& b) {
    return (a.x < b.x);
}

std::map<std::pair<size_t, bits>, std::vector<fs::path>> get(std::string directory) {
    std::map<std::pair<size_t, bits>, std::vector<fs::path>> input;

    file_data& file_data = file_data.get_instance();
    file_data._SHA256.clear();

    for (const auto& entry : fs::directory_iterator(directory)) {
        fs::path path = entry.path();
        if (fs::is_directory(path)) {
            continue;
        }

        file_data._SHA256[path] = sha256_file(path);
        size_t _size = fs::file_size(path);

        std::ifstream in(path, std::ios::in | std::ios::binary);

        char x, y, z;
        in.read((char *) &x, sizeof (x));
        in.seekg(_size / 2);
        in.read((char *) &y, sizeof (y));
        in.seekg(0, in.end);
        in.read((char *) &z, sizeof (z));

        bits bit(x, y, z);

        std::pair<size_t, bits> p(_size, bit);

        input[p].push_back(path);
    }

    return input;
}


QStringList read(std::string directory) {
    std::map<std::pair<size_t, bits>, std::vector<fs::path>> input = get(directory);
    QStringList List;
    file_data &file_data = file_data.get_instance();
    file_data._KEYS.clear();
    file_data._FILES = input;

    for (auto row : input) {
        std::pair<size_t, bits> key = row.first;
        for (auto file : row.second) {
            std::string size = std::to_string(fs::file_size(file));
            std::string name = file.filename();
            file_data._KEYS[name] = key;
            QString QName = QString::fromStdString(name);
            List << QName;
        }
    }

    return List;
}

QStringList read(QString directory)
{
    std::string str = directory.toStdString();
    return read(str);
}
