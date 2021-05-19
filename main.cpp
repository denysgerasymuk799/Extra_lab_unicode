#include <string>
#include <iostream>
#include <fstream>
#include <vector>


#include "inc/count_code_points.hpp"


int main() {
    // cycle for reading test files
    // test each task for UTF-8, UTF-16, UTF-32 on 3 test files: 2 simple and 1 difficult,
    // + without BOM
    std::fstream::pos_type size;
    char *memblock;

    std::vector<std::string> paths_test_files = {"task1_test_UTF16_big.txt"};

    std::string files_dir = "../files/";
    std::string toread = files_dir + "test.txt";
    std::ifstream file(toread, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char[size];

        // TODO: test
        // skip BOM
        file.seekg(2, std::ios::beg);
        size -= 2;

        file.read (memblock, size);
        file.close();

        std::cout << "the complete file content is in memory" << std::endl;

        size_t tohexed = utf16_my_count_code_points(memblock, true);

        std::cout << tohexed << std::endl;
    }
    
    // write function for each task 
    // check with results of existing libraries
    
    // UTF-8
    // https://en.wikipedia.org/wiki/UTF-8#Examples

    // Task 1. UTF-8
////    char *test_string = "Hello World!";
////    char *test_string = "你们好āa";
//    char *test_string = "¢"; // length(test_string) = 2, but code point = 1
////    char *test_string = "$";
//    std::cout << "Number of code points in test_string" << std::endl;
//    std::cout << strlen(test_string) << std::endl;
//
//    std::cout << "print_codepoints -- " << print_codepoints(test_string) << std::endl;
//    std::cout << "my_print_codepoints -- " << my_print_codepoints(test_string) << std::endl;


    //! Task 1. UTF-16
    // https://en.wikipedia.org/wiki/UTF-16

    // https://stackoverflow.com/questions/10504044/correctly-reading-a-utf-16-text-file-into-a-string-without-external-libraries
    // open as a byte stream
//    std::wifstream fin("../test.txt", std::std::ios::binary);
//
//     apply BOM-sensitive UTF-16 facet
//    fin.imbue(std::locale(fin.getloc(),
//                          new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));
//    // read
//    for(wchar_t c; fin.get(c); )
//        std::cout << std::showbase << std::hex << c << '\n';


    return 0;
}
