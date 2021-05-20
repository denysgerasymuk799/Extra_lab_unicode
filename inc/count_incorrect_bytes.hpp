#ifndef LAB_UNICODE_COUNT_INCORRECT_BYTES_H
#define LAB_UNICODE_COUNT_INCORRECT_BYTES_H


#include <iostream>
#include <vector>

#include "configuration.hpp"
#include "utils.hpp"


void find_incorrect_bytes(Config &config_params, std::string &file_string, bool &without_BOM);

void utf8_my_find_incorrect_bytes_LE(std::string &file_string, incorrect_bytes_vector &incorrect_bytes);

void utf16_my_find_incorrect_bytes_LE(std::string &file_string, int &skip_BOM,
                                      incorrect_bytes_vector &incorrect_bytes);

void utf32_my_find_incorrect_bytes_LE(std::string &file_string, int &skip_BOM,
                                   incorrect_bytes_vector &incorrect_bytes);

#endif //LAB_UNICODE_COUNT_INCORRECT_BYTES_H
