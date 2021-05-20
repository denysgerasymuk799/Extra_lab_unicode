// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <bitset>

#include "inc/count_code_points.hpp"
#include "inc/count_code_units.hpp"
#include "inc/count_incorrect_bytes.hpp"
#include "inc/configuration.hpp"
#include "inc/utils.hpp"


int main(int argc, char* argv[]) {
    Config config_params;
    read_config_params(config_params, argc, argv);

    std::ifstream file(config_params.test_file_path, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Can not read file for testing. Check if you input correct path in configs/config.dat" << std::endl;
        exit(ERROR_CONFIG);
    }

    file.seekg(0, std::ios::beg);

    auto file_string = [&file]{
        std::ostringstream ss{};
        ss << file.rdbuf();
        return ss.str();
    }();

    file.close();

    find_file_endianness(config_params, file_string);
    if (config_params.executed_from == "from_main") {
        std::cout << "config_params.endianness -- " << config_params.endianness << std::endl;
        std::cout << "config_params.encoding -- " << config_params.encoding << std::endl;
    }

    bool without_BOM = false;
    if (config_params.num_task == 1) {
        find_num_code_units(config_params, file_string, without_BOM);
    }
    else if (config_params.num_task == 2) {
        find_num_code_points(config_params, file_string, without_BOM);
    }
    else if (config_params.num_task == 3) {
        find_incorrect_bytes(config_params, file_string, without_BOM);
    }
    else {
        std::cerr << "Set up correct number for config_params.num_task ('1' or '2' or '3')" << std::endl;
    };

    if (without_BOM && config_params.executed_from == "from_main") {
        std::cout << "\nFile is not UTF-8 and does not have a BOM. When file without BOM, \n"
                     "so config_params.endianness and config_params.encoding stay default. And really they do not matter for files without BOM" << std::endl;
    }

    return 0;
}
