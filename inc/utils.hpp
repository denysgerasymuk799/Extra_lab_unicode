#ifndef LAB_UNICODE_UTILS_H
#define LAB_UNICODE_UTILS_H

#include <bitset>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "configuration.hpp"
#include "process_config.hpp"


void read_config_params(Config &config_params, int argc, char* argv[]);

void save_results_in_file(Config &config_params, size_t &num_code_points);

void save_incorrect_bytes_in_file(Config &config_params, incorrect_bytes_vector &incorrect_bytes);

void find_file_endianness(Config &config_params, std::string &file_string);

bool check_if_file_utf_8(std::string &file_string);

std::string int_to_hex_str( int i );

#endif //LAB_UNICODE_UTILS_H
