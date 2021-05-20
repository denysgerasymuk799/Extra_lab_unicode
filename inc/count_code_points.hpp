#ifndef LAB_UNICODE_COUNT_CODE_POINTS_H
#define LAB_UNICODE_COUNT_CODE_POINTS_H

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
#include <vector>
#include <boost/regex/pending/unicode_iterator.hpp>
#include <iomanip>
#include <exception>


#include "../inc/configuration.hpp"


void find_num_code_points(Config &config_params, std::string &file_string, bool &without_BOM);

size_t utf8_my_count_code_points(std::vector<char> &vec);

size_t utf8_count_code_points(std::string input);

size_t utf16_my_count_code_points_LE(const std::string& s, bool upper_case, int &skip_BOM_id);

size_t utf16_my_count_code_points_BE(const std::string& s, bool upper_case, int &skip_BOM_idx);

size_t utf32_my_count_code_points(const std::string& s, bool upper_case, int &skip_BOM_idx);

#endif //LAB_UNICODE_COUNT_CODE_POINTS_H
