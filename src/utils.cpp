#include <iostream>

#include "../inc/utils.hpp"


void read_config_params(Config &config_params, int argc, char* argv[]) {
    std::cout << "argc -- " << argc << std::endl;

    if (argc == 1) {
        // TODO: in config
//        config_params.num_task = "3";
//        config_params.encoding = "utf-32";
////        config_params.test_file_path = "../files/task_test_UTF-32_BE_small2_without_BOM.txt";
//        config_params.test_file_path = "../files/files_with_errors/task_test_UTF-16_error_small2.txt";
//        config_params.path_save_results = "../results/res.txt";

        std::string config_path = "../config.dat";
        read_config(config_path, config_params);
        config_params.executed_from = "from_main";
        argc = 6;

    } else {
        if (argc != 6){
            std::cerr << "Incorrect input of parameters in terminal" << std::endl;
            exit(ERROR_CONFIG);
        }

        config_params.num_task = std::stoi(argv[1]);
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


void save_incorrect_bytes_in_file(Config &config_params, incorrect_bytes_vector &incorrect_bytes) {
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

    out_file << "------" << config_params.test_file_path << "\n";
    for (auto & incorrect_byte : incorrect_bytes) {
        out_file << "code unit: " << incorrect_byte[0] << "\n";
        out_file << "positions: ";

        for (int j = 1; j < incorrect_byte.size(); j++) {
            out_file << incorrect_byte[j] << "  ";
        }
        out_file << "\n\n\n";
    }
}


void find_file_endianness(Config &config_params, std::string &file_string) {
    int utf32_BOM_LE[4] = {0xff, 0xfe, 0x00, 0x00};
    int num_occurrences = 0;

    // utf-8 does not have such types as LE or BE;
    // this format is always BE -- https://stackoverflow.com/questions/3833693/isn-t-on-big-endian-machines-utf-8s-byte-order-different-than-on-little-endian
    // also it is clear when we print first bytes of utf-8 file
    config_params.encoding = "utf-8";
    config_params.endianness = "LE";

    for (int i = 0; i < 8; i++) {
        std::cout << std::hex << (file_string[i] & 0xff) << " ";
    }
    std::cout << std::endl;
//    std::cout << std::hex << "(s[1] & 0xff) -- " << (s[1] & 0xff) << std::endl;

    // refer to Byte order marks by encoding section in https://en.wikipedia.org/wiki/Byte_order_mark
    if ((file_string[0] & 0xff) == utf32_BOM_LE[0]) {
        for (std::string::size_type i = 0; i < 4; ++i) {
            int z = file_string[i] & 0xff;
            if (z == utf32_BOM_LE[i]) {
                num_occurrences++;
            } else {
                break;
            }

            if (num_occurrences == 2 && i == 1) {
                config_params.endianness = "LE";
                config_params.encoding = "utf-16";
            }
        }

        if (num_occurrences == 4) {
            config_params.endianness = "LE";
            config_params.encoding = "utf-32";
        }
    } else {
        if ((file_string[0] & 0xff) == utf32_BOM_LE[3] && (file_string[1] & 0xff) == utf32_BOM_LE[2] &&
            (file_string[2] & 0xff) == utf32_BOM_LE[1] && (file_string[3] & 0xff) == utf32_BOM_LE[0]) {
            config_params.endianness = "BE";
            config_params.encoding = "utf-32";
        }
        else if ((file_string[0] & 0xff) == utf32_BOM_LE[1] && (file_string[1] & 0xff) == utf32_BOM_LE[0]) {
            config_params.endianness = "BE";
            config_params.encoding = "utf-16";
        }
    }
}


bool check_if_file_utf_8(std::string &file_string) {
    size_t length = file_string.length();
    size_t i = 0;

    while(true) {
        if (i >= length)
            break;

        std::bitset<8> binary_char1(file_string[i]);
        if ((binary_char1 >> 7) == 0) {
            i++;
            continue;
        }
        else if ((binary_char1 >> 5) == 6) {
            if (i + 1 < length) {
                std::bitset<8> binary_char2(file_string[i + 1]);
                if ((binary_char2 >> 6) == 2) {
                    i += 2;
                    continue;
                }
            }
        }
        else if ((binary_char1 >> 4) == 14) {
            if (i + 2 < length) {
                std::bitset<8> binary_char2(file_string[i + 1]);
                std::bitset<8> binary_char3(file_string[i + 2]);
                if ((binary_char2 >> 6) == 2 && (binary_char3 >> 6) == 2) {
                    i += 3;
                    continue;
                }
            }
        }
        else if ((binary_char1 >> 3) == 30) {
            if (i + 3 < length) {
                std::bitset<8> binary_char2(file_string[i + 1]);
                std::bitset<8> binary_char3(file_string[i + 2]);
                std::bitset<8> binary_char4(file_string[i + 3]);
                if ((binary_char2 >> 6) == 2 && (binary_char3 >> 6) == 2 && (binary_char4 >> 6) == 2) {
                    i += 4;
                    continue;
                }
            }
        }

        std::cout << "Incorrect binary char -- " << binary_char1 << std::endl;
        return false;
    }

    return true;
}


//template< typename T >
std::string int_to_hex_str(int i) {
    std::stringstream stream;
    stream << std::setfill ('0') << std::setw(2) << std::hex << i;
    return stream.str();
}
