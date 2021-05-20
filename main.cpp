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
    // TODO:
    //  try catch for utf-8 add
    //  why file with English text does not work
    //  add error prints
    //  TODOs from generals labs check
    //  test python script
    //  check tests -- cycle for reading test files
    //   test each task for UTF-8, UTF-16, UTF-32 on 3 test files: 2 simple and 1 difficult,
    //   + without BOM + big and little endian

    // TODO: after add
    //  video explanation
    //  PVS studio

    Config config_params;
    read_config_params(config_params, argc, argv);

    std::ifstream file(config_params.test_file_path, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Can not read file for testing" << std::endl;
        exit(ERROR_CONFIG);
    }

    // TODO: test
    file.seekg(0, std::ios::beg);

    auto file_string = [&file]{
        std::ostringstream ss{};
        ss << file.rdbuf();
        return ss.str();
    }();

    file.close();

//    for (char i : file_string) {
//        std::cout << std::bitset<8>(i) << " ";
//    }
//    std::cout << std::endl;

//    if (config_params.endianness != "BE" && config_params.endianness != "LE") {
    find_file_endianness(config_params, file_string);
//    }

    std::cout << "config_params.endianness -- " << config_params.endianness << std::endl;
    std::cout << "config_params.encoding -- " << config_params.encoding << std::endl;

    bool without_BOM = false;
    if (config_params.num_task == 1) {
        find_num_code_units(config_params, file_string, without_BOM);
    }
    else if (config_params.num_task == 2) {
        find_num_code_points(config_params, file_string, without_BOM);
    }
    else if (config_params.num_task == 3) {
        find_incorrect_bytes(config_params, file_string, without_BOM);
    } else {
        std::cerr << "Set up correct number for config_params.num_task ('1' or '2' or '3')" << std::endl;
    };

    if (without_BOM) {
        std::cout << "File is not UTF-8 and does not have a BOM" << std::endl;
    }

    return 0;
}
