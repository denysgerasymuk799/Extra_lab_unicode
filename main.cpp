#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "inc/count_code_points.hpp"
#include "inc/configuration.hpp"



struct Config {
    std::string num_task;
    std::string encoding;
    std::string endianness;
    std::string test_file_path;
    std::string path_save_results;
    std::string executed_from;
};


void read_config_params(Config &config_params, int argc, char* argv[]) {
    std::cout << "argc -- " << argc << std::endl;

    if (argc == 1) {
        config_params.num_task = "1";
        config_params.encoding = "utf-8";
        config_params.endianness = "LE";
        config_params.test_file_path = "../files/task_test_UTF-8_small1.txt";
        config_params.path_save_results = "../res.txt";
        config_params.executed_from = "from_main";
        argc = 7;

    } else {
        if (argc != 7){
            std::cerr << "Incorrect input of parameters in terminal" << std::endl;
            exit(ERROR_CONFIG);
        }

        config_params.num_task = argv[1];
        config_params.encoding = argv[2];
        config_params.endianness = argv[3];
        config_params.test_file_path = argv[4];
        config_params.path_save_results = argv[5];
        config_params.executed_from = argv[6];
    }
}


int main(int argc, char* argv[]) {
    // TODO:
    //  why file with English text does not work
    //  add error prints
    //  TODOs from generals labs check
    //  test python script
    //  check tests -- cycle for reading test files
    //   test each task for UTF-8, UTF-16, UTF-32 on 3 test files: 2 simple and 1 difficult,
    //   + without BOM + big and little endian

    Config config_params;
    read_config_params(config_params, argc, argv);

    std::ifstream file(config_params.test_file_path, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Can not read file for testing" << std::endl;
        exit(ERROR_CONFIG);
    }

    int BOM_num_bytes = 0;
    if (config_params.encoding == "utf-16") {
        BOM_num_bytes = 2;
    }
    else if (config_params.encoding == "utf-32") {
        BOM_num_bytes = 4;
    }

    // TODO: test
    // skip BOM
    file.seekg(BOM_num_bytes, std::ios::beg);

    auto s = [&file]{
        std::ostringstream ss{};
        ss << file.rdbuf();
        return ss.str();
    }();

    file.close();

    size_t num_code_points = 0;
    if (config_params.encoding == "utf-8") {
        std::vector<char> chars(s.begin(), s.end());
        num_code_points = utf8_my_count_code_points(chars);
    }
    else if (config_params.encoding == "utf-16") {
        num_code_points = utf16_my_count_code_points(s, true);
    }
    else if (config_params.encoding == "utf-32") {
        num_code_points = utf32_my_count_code_points(s, true);
    }

    std::cout << "Number of code point" << std::endl;
    std::cout << std::dec << config_params.encoding << " my_count_code_points -- " << num_code_points << std::endl;

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

    return 0;
}
