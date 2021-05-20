// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "../inc/count_code_units.hpp"


void find_num_code_units(Config &config_params, std::string &file_string, bool &without_BOM) {
    size_t num_code_units = 0;
    int skip_BOM_idx;
    int bytes_in_code_unit;
    if (config_params.encoding == "utf-8") {
        if (check_if_file_utf_8(file_string)) {
            bytes_in_code_unit = 1;
            num_code_units = file_string.length() / bytes_in_code_unit;
        } else {
            without_BOM = true;
        }
    }
    else if (config_params.encoding == "utf-16") {
        skip_BOM_idx = 2;
        bytes_in_code_unit = 2;
        num_code_units = (file_string.length() - skip_BOM_idx) / bytes_in_code_unit;
    }
    else if (config_params.encoding == "utf-32") {
        skip_BOM_idx = 4;
        bytes_in_code_unit = 4;
        num_code_units = (file_string.length() - skip_BOM_idx) / bytes_in_code_unit;
    }

    if (config_params.executed_from == "from_main") {
        std::cout << "Filename " << config_params.test_file_path << std::endl;
        std::cout << std::dec << config_params.encoding << " Number of code units -- " << num_code_units << std::endl;
    }

    save_results_in_file(config_params, num_code_units);
}
