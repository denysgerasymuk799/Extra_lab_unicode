#ifndef LAB_UNICODE_CONFIGURATION_H
#define LAB_UNICODE_CONFIGURATION_H

//#define DEBUG_MODE
#define ERROR_CONFIG 1
#define IO_READ_ERR -2
#define INVALID_ARG 3
#define CFG_VALUE_ERROR 4
#define INCORRECT_INPUT 5

typedef std::vector<std::vector<std::string>> incorrect_bytes_vector;

struct Config {
    int num_task;
    std::string encoding;
    std::string test_file_path;
    std::string path_save_results;
    std::string executed_from;
    std::string endianness;
};


#endif //LAB_UNICODE_CONFIGURATION_H
