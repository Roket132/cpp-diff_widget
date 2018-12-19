#ifndef FILE_DATA_H
#define FILE_DATA_H

#include "reader.h"

#include <algorithm>
#include <experimental/filesystem>
#include <map>

namespace fs = std::experimental::filesystem;

class file_data
{
private:
    file_data();
    file_data(const file_data&);
    file_data& operator=(file_data&);

public:
    std::map<std::pair<size_t, bits>, std::vector<fs::path>> _FILES;
    std::map<std::string, std::pair<size_t, bits>> _KEYS;
    std::map<fs::path, std::string> _SHA256;

    static file_data& get_instance() {
        static file_data instance;
        return instance;
    }
};

#endif // FILE_DATA_H
