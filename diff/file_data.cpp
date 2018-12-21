#include "file_data.h"


file_data::file_data() {}

file_data &file_data::get_instance()
{
    static file_data instance;
    return instance;
}

