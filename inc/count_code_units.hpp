#ifndef LAB_UNICODE_COUNT_CODE_UNITS_H
#define LAB_UNICODE_COUNT_CODE_UNITS_H

#include <iostream>
#include <vector>

#include "configuration.hpp"
#include "utils.hpp"


void find_num_code_units(Config &config_params, std::string &file_string, bool &without_BOM);

#endif //LAB_UNICODE_COUNT_CODE_UNITS_H
