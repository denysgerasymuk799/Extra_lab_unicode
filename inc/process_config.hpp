#ifndef LAB_UNICODE_PROCESS_CONFIG_H
#define LAB_UNICODE_PROCESS_CONFIG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
#include <cassert>
#include <locale>

#include "configuration.hpp"


void read_config(std::string &filename, Config &setup);

void extract_program_args(std::unordered_map<std::string, std::string> &config_data, Config &setup);

void split_str(std::string s, const std::string &delim, std::vector<std::string> &arr);

bool is_number(std::string &str);

#endif //LAB_UNICODE_PROCESS_CONFIG_H
