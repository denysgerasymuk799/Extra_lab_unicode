#ifndef LAB_UNICODE_UTILS_H
#define LAB_UNICODE_UTILS_H

#include "../inc/configuration.hpp"
#include <bitset>


void find_file_endianness(Config &config_params, std::string &file_string);

bool check_if_file_utf_8(std::string &file_string);

#endif //LAB_UNICODE_UTILS_H
