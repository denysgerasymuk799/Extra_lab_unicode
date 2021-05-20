#include <iostream>

#include "../inc/utils.hpp"


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
