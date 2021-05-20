// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <cmath>

#include "../inc/process_config.hpp"


void read_config(std::string &filename, Config &setup) {
    std::ifstream cfg(filename);

    if (!cfg.is_open()) {
        exit(IO_READ_ERR);
    }

    std::string line{};
    std::unordered_map<std::string, std::string> config_data;
    while (std::getline(cfg, line)) {
        /* If the line contains an equals operator -- we treat it as valid data
         * std::string::npos means "until the end of the string". */
        if (line.find('=') != std::string::npos) {
            // Split data in line into a key and value
            std::istringstream iss{ line };
            std::string key{}, value{};

            /* Operation std::getline(iss, id, ':') extracts a string from the std::istringstream
             * and assigns it to variable "id".*/
            if (std::getline(std::getline(iss, key, '=') >> std::ws, value)) {
                key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
                config_data[key] = value;
            }
        }
    }
    cfg.close();
    extract_program_args(config_data, setup);
}


void extract_program_args(std::unordered_map<std::string, std::string> &config_data, Config &setup) {
    try {
        assert(is_number(config_data.at("num_task")) && "<num_task> must be a number.");
        setup.num_task = std::stoi(config_data.at("num_task"));
        assert((setup.num_task >= 1 && setup.num_task <= 3) && "<num_task> should be in range [1, 3]");

        try {
            setup.encoding = config_data.at("encoding");
        } catch(std::out_of_range &e) {
            setup.encoding = "-";
        }

        setup.test_file_path = config_data.at("test_file_path");
        if(std::filesystem::is_directory(config_data["test_file_path"])) {
            std::cout << "No file found by path -- <test_file_path>: " << setup.test_file_path << std::endl;
            exit(INCORRECT_INPUT);
        }

        setup.path_save_results = config_data.at("path_save_results");
        if(std::filesystem::is_directory(config_data["path_save_results"])) {
            std::cout << "No file found by path -- <path_save_results>: " << setup.path_save_results << std::endl;
            exit(INCORRECT_INPUT);
        }

    } catch (std::out_of_range &e) {
        std::cout << "Missing a program setup argument in the config!\n" << std::endl;
        exit(CFG_VALUE_ERROR);
    } catch (std::invalid_argument &e) {
        std::cout << "Invalid argument! All config arguments should be numbers!" << std::endl;
        exit(CFG_VALUE_ERROR);
    }
}


bool is_number(std::string &str) {
    for (char i : str) {
        if (!isdigit(i)) { return false; }
    }
    return true;
}

