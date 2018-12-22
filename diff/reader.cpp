#include "reader.h"
#include "mainwindow.h"
#include "file_data.h"
#include "sha256.h"
#include "ui_mainwindow.h"

#include <QApplication>



size_t MAX_SZ = 3.2e6;

bits::bits() :x(0), y(0), z(0) {}

bits::bits(char x, char y, char z) : x(x), y(y), z(z) {}

bool operator==(bits const& a, bits const& b) {
    return (a.x == b.x);
}
bool operator<(bits const& a, bits const& b) {
    return (a.x < b.x);
}

std::size_t number_of_files_in_directory(fs::path path)
{
    return std::distance(fs::recursive_directory_iterator(path), fs::recursive_directory_iterator{});
}

std::map<std::pair<size_t, bits>, std::vector<fs::path>> get(std::string directory, QProgressBar *bar, int mode) {
    std::map<std::pair<size_t, bits>, std::vector<fs::path>> input;
    int cnt = 0;
    double add_progress = 100.0 / number_of_files_in_directory(directory) * 1.0;

    file_data& file_data = file_data.get_instance();
    std::map<fs::path, std::string> if_clear_sha256;
    std::map<fs::path, fs::file_time_type> if_clear_time_modif;
    bool clear = false;
    if (file_data._LAST_MODIF.size() > MAX_SZ) {
        clear = true;
    }
    if (file_data._FAST_HASH.size() > MAX_SZ) {
        file_data._FAST_HASH.clear();
    }

    bar->setValue(int(cnt * add_progress));

    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        try {
            fs::path path = entry.path();
            if (fs::is_directory(path) || fs::is_block_file(path) || fs::is_symlink(path)) {
                continue;
            }

            if (mode == 2) {
                file_data._FAST_HASH[path] = fast_hash(path);
            }

            if (mode == 1) {
                bool was = !file_data._SHA256.count(path);
                if (clear) {
                    if (was) {
                        if_clear_sha256[path] = file_data._SHA256[path];
                        if_clear_time_modif[path] = file_data._LAST_MODIF[path];
                    } else {
                        file_data._SHA256[path] = sha256_file(path);
                        file_data._LAST_MODIF[path] = fs::last_write_time(path);
                    }
                } else {
                    if (was && fs::last_write_time(path) != file_data._LAST_MODIF[path]) {
                        file_data._SHA256[path] = sha256_file(path);
                        file_data._LAST_MODIF[path] = fs::last_write_time(path);
                    }
                }
            }


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
        } catch (fs::filesystem_error e) {
            std::cout << e.what() << std::endl;
        }
        cnt++;
        bar->setValue(int(cnt * add_progress));
    }

    if (clear) {
        std::swap(file_data._SHA256, if_clear_sha256);
        std::swap(file_data._LAST_MODIF, if_clear_time_modif);
    }

    return input;
}


QStringList read(std::string directory, QProgressBar *bar, int mode) {
    std::map<std::pair<size_t, bits>, std::vector<fs::path>> input = get(directory, bar, mode);
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

QStringList read(QString directory, QProgressBar *bar, int mode)
{
    std::string str = directory.toStdString();
    return read(str, bar, mode);
}

QStringList read_update(std::string directory, QProgressBar *bar, int mode)
{
    return read(directory, bar, mode);
}

QStringList get_same(std::string name, int mode)
{
    file_data &file_data = file_data.get_instance();
    std::pair<size_t, bits> key = file_data._KEYS[name];
    auto row = file_data._FILES[key];

    fs::path item_file;
    for (auto it : row) {
        if (it.filename() == name) {
            item_file = it;
            break;
        }
    }

    std::string sha256 = (mode == 1 ? file_data._SHA256[item_file] : file_data._FAST_HASH[item_file]);
    std::vector<fs::path> same;

    for (auto it : row) {
        if (it.filename() != name) {
            std::string sha256_it = (mode == 1 ? file_data._SHA256[it] : file_data._FAST_HASH[it]);
            if (sha256 == sha256_it) {
                same.push_back(it);
            }
        }
    }

    QStringList List;

    for (auto it : same) {
        List << QString::fromStdString(it.filename());
    }

    return List;
}
