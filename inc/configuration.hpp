#ifndef LAB_UNICODE_CONFIGURATION_H
#define LAB_UNICODE_CONFIGURATION_H

//#define DEBUG_MODE
#define ERROR_CONFIG -1


struct Config {
    std::string num_task;
    std::string encoding;
    std::string test_file_path;
    std::string path_save_results;
    std::string executed_from;
    std::string endianness;
};


#endif //LAB_UNICODE_CONFIGURATION_H
