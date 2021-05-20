#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <bitset>

#include "inc/count_code_points.hpp"
#include "inc/configuration.hpp"
#include "inc/utils.hpp"


void read_config_params(Config &config_params, int argc, char* argv[]) {
    std::cout << "argc -- " << argc << std::endl;

    if (argc == 1) {
        config_params.num_task = "2";
        config_params.encoding = "utf-32";
//        config_params.test_file_path = "../files/task_test_UTF-32_BE_small2_without_BOM.txt";
        config_params.test_file_path = "../files/task_test_UTF-16_big.txt";
        config_params.path_save_results = "../results/res.txt";
        config_params.executed_from = "from_main";
        argc = 6;

    } else {
        if (argc != 6){
            std::cerr << "Incorrect input of parameters in terminal" << std::endl;
            exit(ERROR_CONFIG);
        }

        config_params.num_task = argv[1];
        config_params.encoding = argv[2];
        config_params.test_file_path = argv[3];
        config_params.path_save_results = argv[4];
        config_params.executed_from = argv[5];
    }
}


void save_results_in_file(Config &config_params, size_t &num_code_points) {
    std::ofstream out_file;

    // if module is run from python script
    if (config_params.executed_from == "from_python") {
        out_file.open(config_params.path_save_results, std::ios_base::app);
    }
    else if (config_params.executed_from == "from_python_start")
        out_file.open(config_params.path_save_results, std::ios_base::trunc);
    else
        out_file.open(config_params.path_save_results);

    if (!out_file) {
        out_file.open(config_params.path_save_results, std::ios_base::out);
    }

    out_file << std::dec << config_params.test_file_path << ": " << num_code_points << "\n";
}


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

    Config config_params;
    read_config_params(config_params, argc, argv);

    std::ifstream file(config_params.test_file_path, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Can not read file for testing" << std::endl;
        exit(ERROR_CONFIG);
    }

//    int BOM_num_bytes = 0;
//
//    if (config_params.encoding == "utf-16") {
//        BOM_num_bytes = 0;
//    }
//    else if (config_params.encoding == "utf-32") {
//        BOM_num_bytes = 0;
//    }

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

    size_t num_code_points = 0;
    int skip_BOM_idx;
    bool without_BOM = false;
    if (config_params.encoding == "utf-8") {
        if (check_if_file_utf_8(file_string)) {
            std::vector<char> chars(file_string.begin(), file_string.end());
            num_code_points = utf8_my_count_code_points(chars);
        } else {
            without_BOM = true;
        }
    }
    else if (config_params.encoding == "utf-16") {
        skip_BOM_idx = 2;
        if (config_params.endianness == "BE")
            num_code_points = utf16_my_count_code_points_BE(file_string, true, skip_BOM_idx);
        else
            num_code_points = utf16_my_count_code_points(file_string, true, skip_BOM_idx);
    }
    else if (config_params.encoding == "utf-32") {
        skip_BOM_idx = 4;
        num_code_points = utf32_my_count_code_points(file_string, true, skip_BOM_idx);
    }

    std::cout << "Filename " << config_params.test_file_path << std::endl;
    std::cout << std::dec << config_params.encoding << " Number of code point -- " << num_code_points << std::endl;


//    // if file without BOM, so in utf8_my_count_code_points() would occur
//    // exception and would be set up num_code_points = -1
//    if (num_code_points == -1)
//        return 0;

    if (without_BOM) {
        std::cout << "File is not UTF-8 and does not have a BOM" << std::endl;
    }

    save_results_in_file(config_params, num_code_points);
    return 0;
}
