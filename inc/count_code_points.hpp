#ifndef LAB_UNICODE_COUNT_CODE_POINTS_H
#define LAB_UNICODE_COUNT_CODE_POINTS_H

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
#include <boost/regex/pending/unicode_iterator.hpp>
#include <iomanip>


#define DEBUG_MODE


size_t utf8_my_count_code_points(char *s);

size_t utf8_count_code_points(std::string input);

size_t utf16_my_count_code_points(const std::string& s, bool upper_case);

#endif //LAB_UNICODE_COUNT_CODE_POINTS_H
