#include "../inc/count_incorrect_bytes.hpp"


void find_incorrect_bytes(Config &config_params, std::string &file_string, bool &without_BOM) {
    size_t num_code_points = 0;
    int skip_BOM_idx;
    incorrect_bytes_vector incorrect_bytes;
    if (config_params.encoding == "utf-8") {
        utf8_my_find_incorrect_bytes_LE(file_string, incorrect_bytes);
    }
    else if (config_params.encoding == "utf-16") {
        skip_BOM_idx = 2;
        if (config_params.endianness == "LE") {
            utf16_my_find_incorrect_bytes_LE(file_string, skip_BOM_idx, incorrect_bytes);
        } else {
            std::cerr << "For UTF-16 BE finding incorrect bytes is not added yet. But idea pretty the same as for UTf-16 LE" << std::endl;
        }
    }
    else if (config_params.encoding == "utf-32") {
        skip_BOM_idx = 4;
        if (config_params.endianness == "LE") {
            utf32_my_find_incorrect_bytes_LE(file_string, skip_BOM_idx, incorrect_bytes);
        } else {
            std::cerr << "For UTF-32 BE finding incorrect bytes is not added yet. But idea pretty the same as for UTf-32 LE" << std::endl;
        }
    }

    std::cout << "Filename " << config_params.test_file_path << std::endl;
    std::cout << std::dec << config_params.encoding << " Number of code point -- " << num_code_points << std::endl;

    save_incorrect_bytes_in_file(config_params, incorrect_bytes);
}


void utf8_my_find_incorrect_bytes_LE(std::string &file_string, incorrect_bytes_vector &incorrect_bytes) {
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

        if ((binary_char1 >> 5) == 6) {
            if (i + 1 < length) {
                std::bitset<8> binary_char2(file_string[i + 1]);

                if ((binary_char2 >> 6) == 2) {
                    i += 2;
                    continue;
                } else {
                    std::vector<std::string> incorrect_byte;
                    std::string hex_str = binary_char2.to_string();
                    incorrect_byte.push_back(hex_str + " (bitset must start with 10)");
                    incorrect_byte.push_back(std::to_string(i + 1));

                    incorrect_bytes.push_back(incorrect_byte);
                    i += 2;
                }
            } else {
                std::vector<std::string> incorrect_byte;
                std::string hex_str = binary_char1.to_string();
                incorrect_byte.push_back(hex_str + " (this bitset starts with 110, but not enough bytes after it. It must be 1 byte after)");
                incorrect_byte.push_back(std::to_string(i));

                incorrect_bytes.push_back(incorrect_byte);
                i++;
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

                if ((binary_char2 >> 6) != 2) {
                    std::vector<std::string> incorrect_byte;
                    std::string hex_str = binary_char2.to_string();
                    incorrect_byte.push_back(hex_str + " (second bitset, after first bitset which starts with 1110, must start with 10)");
                    incorrect_byte.push_back(std::to_string(i + 1));

                    incorrect_bytes.push_back(incorrect_byte);
                }

                if ((binary_char3 >> 6) != 2) {
                    std::vector<std::string> incorrect_byte;
                    std::string hex_str = binary_char3.to_string();
                    incorrect_byte.push_back(hex_str + " (third bitset, after first bitset which starts with 1110, must start with 10)");
                    incorrect_byte.push_back(std::to_string(i + 2));

                    incorrect_bytes.push_back(incorrect_byte);
                }
                i += 3;

            } else {
                std::vector<std::string> incorrect_byte;
                std::string hex_str = binary_char1.to_string();
                incorrect_byte.push_back(hex_str + " (this bitset starts with 1110, but not enough bytes after it. It must be 2 bytes after)");
                incorrect_byte.push_back(std::to_string(i));

                incorrect_bytes.push_back(incorrect_byte);
                i++;
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

                if ((binary_char2 >> 6) != 2) {
                    std::vector<std::string> incorrect_byte;
                    std::string hex_str = binary_char2.to_string();
                    incorrect_byte.push_back(hex_str + " (second bitset, after first bitset which starts with 11110, must start with 10)");
                    incorrect_byte.push_back(std::to_string(i + 1));

                    incorrect_bytes.push_back(incorrect_byte);
                }

                if ((binary_char3 >> 6) != 2) {
                    std::vector<std::string> incorrect_byte;
                    std::string hex_str = binary_char3.to_string();
                    incorrect_byte.push_back(hex_str + " (third bitset, after first bitset which starts with 11110, must start with 10)");
                    incorrect_byte.push_back(std::to_string(i + 2));

                    incorrect_bytes.push_back(incorrect_byte);
                }

                if ((binary_char4 >> 6) != 2) {
                    std::vector<std::string> incorrect_byte;
                    std::string hex_str = binary_char3.to_string();
                    incorrect_byte.push_back(hex_str + " (forth bitset, after first bitset which starts with 11110, must start with 10)");
                    incorrect_byte.push_back(std::to_string(i + 3));

                    incorrect_bytes.push_back(incorrect_byte);
                }
                i += 4;

            } else {
                std::vector<std::string> incorrect_byte;
                std::string hex_str = binary_char1.to_string();
                incorrect_byte.push_back(hex_str + " (this bitset starts with 11110, but not enough bytes after it. It must be 3 bytes after)");
                incorrect_byte.push_back(std::to_string(i));

                incorrect_bytes.push_back(incorrect_byte);
                i++;
            }
        } else {
            std::vector<std::string> incorrect_byte;
            std::string hex_str = binary_char1.to_string();
            incorrect_byte.push_back(hex_str + " (this bitset starts with invalid first bits, which can not be in UTF-8, or before it there is no main byte, which starts with 11110 or 1110 or 110)");
            incorrect_byte.push_back(std::to_string(i));

            incorrect_bytes.push_back(incorrect_byte);
            i++;
        }
    }
}


void utf16_my_find_incorrect_bytes_LE(std::string &file_string, int &skip_BOM,
                                      incorrect_bytes_vector &incorrect_bytes) {
    size_t length = file_string.length();
    size_t i = skip_BOM;

    int two_bytes[2];
    int low_surrogate_hex, high_surrogate_hex;

    // The low ten bits (also in the range 0x000–0x3FF) are added to 0xDC00 to give the second 16-bit code unit or
    // low surrogate (W2), which will be in the range 0xDC00–0xDFFF
    //
    // The high ten bits in range 0xD800–0xDBFF
    //
    int high_surrogate_limits[2] = {0xd800, 0xdbff};
    int low_surrogate_limits[2] = {0xdc00, 0xdfff};

    while(true) {
        if (i >= length)
            break;

        two_bytes[0] = file_string[i] & 0xff;

        if (i + 1 >= length) {
            std::vector<std::string> incorrect_byte;
            std::string hex_str = int_to_hex_str(two_bytes[0]);
            hex_str += " (after this byte should be one more byte to form a code unit in UTF-16)";
//            std::string hex_str = "DF";
            incorrect_byte.push_back(hex_str);
            incorrect_byte.push_back(std::to_string(i));

            // in UTF-16 it must be always that a code unit consists of 16 bits, so it is incorrect
            // as it has only 8 bits
            incorrect_bytes.push_back(incorrect_byte);
            break;
        }
        else {
            two_bytes[1] = file_string[i + 1] & 0xff;

            high_surrogate_hex = (two_bytes[1] << 8) + two_bytes[0];
            if (high_surrogate_limits[0] <= high_surrogate_hex && high_surrogate_hex <= low_surrogate_limits[1]) {
                if (i + 3 < length) {
                    two_bytes[0] = file_string[i + 2] & 0xff;
                    two_bytes[1] = file_string[i + 3] & 0xff;

                    low_surrogate_hex = (two_bytes[1] << 8) + two_bytes[0];

                    if (! (high_surrogate_limits[0] <= high_surrogate_hex && high_surrogate_hex <= high_surrogate_limits[1]) ) {
                        std::vector<std::string> incorrect_byte;
                        std::string hex_str = int_to_hex_str(high_surrogate_hex);
                        hex_str += " (high_surrogate_hex is not in correct limits)";
                        std::cout << "std::string hex -- " << hex_str << std::endl;
                        incorrect_byte.push_back(hex_str);
//                        incorrect_byte.push_back(int_to_hex_str(high_surrogate_hex));
                        incorrect_byte.push_back(std::to_string(i));
                        incorrect_byte.push_back(std::to_string(i + 1));

                        // high_surrogate_hex is not in correct limits
//                        incorrect_bytes.push_back({int_to_hex_str(high_surrogate_hex), std::to_string(i), std::to_string(i + 1)});
                        incorrect_bytes.push_back(incorrect_byte);
                    }

                    if (! (low_surrogate_limits[0] <= low_surrogate_hex && low_surrogate_hex <= low_surrogate_limits[1]) ) {
                        std::vector<std::string> incorrect_byte;
                        std::string hex_str = int_to_hex_str(low_surrogate_hex);
                        std::cout << "std::string hex -- " << hex_str << std::endl;
                        incorrect_byte.push_back(hex_str + " (low_surrogate_hex is not in correct limits)");
//                        incorrect_byte.push_back(int_to_hex_str(high_surrogate_hex));
                        incorrect_byte.push_back(std::to_string(i + 2));
                        incorrect_byte.push_back(std::to_string(i + 3));

                        // low_surrogate_hex is not in correct limits
                        incorrect_bytes.push_back(incorrect_byte);
                    }

                    i += 4;
                } else {
                    std::vector<std::string> incorrect_byte;
                    std::string hex_str = int_to_hex_str(high_surrogate_hex);
                    std::cout << "std::string hex -- " << hex_str << std::endl;
                    incorrect_byte.push_back(hex_str + " (no low_surrogate_hex after this surrogate hex)");
                    incorrect_byte.push_back(std::to_string(i));
                    incorrect_byte.push_back(std::to_string(i + 1));

                    // after high_surrogate_hex also must be low_surrogate_hex, so it is incorrect
                    // as for this 16-bits code unit it is not true
                    incorrect_bytes.push_back(incorrect_byte);
//                    incorrect_bytes.push_back({int_to_hex_str(high_surrogate_hex), std::to_string(i), std::to_string(i + 1)});

                    i += 2;
                }
            } else {
                i++;
            }
        }
    }
}


void utf32_my_find_incorrect_bytes_LE(std::string &file_string, int &skip_BOM,
                                  incorrect_bytes_vector &incorrect_bytes) {
    size_t length = file_string.length();
    size_t i = skip_BOM;
    int two_bytes[4];

    int mod_file_length = (int) (length - skip_BOM) % 4;
    if (mod_file_length != 0) {
        std::vector<std::string> incorrect_byte;
        int z = file_string[length - 1] & 0xff;
        std::string hex_str = int_to_hex_str(z);
        std::cout << "std::string hex -- " << hex_str << std::endl;
        incorrect_byte.push_back(hex_str + " (UTF-32 file must contain len(bytes) % 4 == 0)");
        incorrect_byte.push_back(std::to_string(length - 1));

        incorrect_bytes.push_back(incorrect_byte);
    }

    int code_unit;
    int max_unicode_code = 0x10ffff;
    while(true) {
        if (i >= length - mod_file_length)
            break;

        two_bytes[0] = file_string[i] & 0xff;
        two_bytes[1] = file_string[i + 1] & 0xff;
        two_bytes[2] = file_string[i + 2] & 0xff;
        two_bytes[3] = file_string[i + 3] & 0xff;

        code_unit = (two_bytes[3] << 24) + (two_bytes[2] << 16) + (two_bytes[1] << 8) + two_bytes[0];

        if (code_unit > max_unicode_code) {
            std::vector<std::string> incorrect_byte;
            std::string hex_str = int_to_hex_str(code_unit);
            incorrect_byte.push_back(hex_str + " (in unicode code of the symbol can not be more than U+10FFFF)");
            incorrect_byte.push_back(std::to_string(i));
            incorrect_byte.push_back(std::to_string(i + 1));
            incorrect_byte.push_back(std::to_string(i + 2));
            incorrect_byte.push_back(std::to_string(i + 3));

            incorrect_bytes.push_back(incorrect_byte);
        }

        i += 4;
    }
}
