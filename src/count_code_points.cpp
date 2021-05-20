#include "../inc/count_code_points.hpp"
#include "../inc/utils.hpp"


void find_num_code_points(Config &config_params, std::string &file_string, bool &without_BOM) {
    size_t num_code_points = 0;
    int skip_BOM_idx;
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
            num_code_points = utf16_my_count_code_points_LE(file_string, true, skip_BOM_idx);
    }
    else if (config_params.encoding == "utf-32") {
        skip_BOM_idx = 4;
        num_code_points = utf32_my_count_code_points(file_string, true, skip_BOM_idx);
    }

    std::cout << "Filename " << config_params.test_file_path << std::endl;
    std::cout << std::dec << config_params.encoding << " Number of code point -- " << num_code_points << std::endl;

    save_results_in_file(config_params, num_code_points);
}


// returns the number of utf8 code points in the buffer at s
size_t utf8_my_count_code_points(std::vector<char> &vec) {
    size_t len = 0;
//    for (; *s; ++s) if ((*s & 0xC0) != 0x80) ++len;
//    try {

    for (char i : vec) {
        if ((i & 0xC0) != 0x80) {
            ++len;
        }
    }

//    } catch(std::exception &e) {
//#ifdef DEBUG_MODE
//        std::cout << "Error in utf8_my_count_code_points(): " << e.what() << std::endl;
//#endif
//        len = -1;
//    }
    return len;
}


size_t utf8_count_code_points(std::string &input) {
    size_t len = 0;
    for(boost::u8_to_u32_iterator<std::string::iterator> it(input.begin()), end(input.end()); it!=end; ++it) {
#ifdef DEBUG_MODE
        std::cout <<"\""<< * it << "\", ";
#endif
        len++;
    }
    std::cout << std::endl;
    return len;
}


size_t utf16_my_count_code_points_LE(const std::string& s, bool upper_case, int &skip_BOM_idx) {
    size_t num_code_points = 0;

#ifdef DEBUG_MODE
    std::cout << "Hex bytes of the file content" << std::endl;
#endif

    int two_bytes[2];
    int two_bytes_hex;

    // The low ten bits (also in the range 0x000–0x3FF) are added to 0xDC00 to give the second 16-bit code unit or
    // low surrogate (W2), which will be in the range 0xDC00–0xDFFF
    //
    // The high ten bits in range 0xD800–0xDBFF
    //
    // Another symbols in range U+0000 to U+D7FF and U+E000 to U+FFFF
    int high_ten_bits_limit = 0xdc00;
    int upper_limits_simple_symbols[2] = {0xe000, 0xffff};


    for (std::string::size_type i = skip_BOM_idx; i < s.length(); ++i) {
        int z = s[i] & 0xff;

        // when for-cycle was started from 0 index, so
        // i % 2 == 0 is odd number
        // i % 2 == 1 is even number
        if (i % 2 == 0) {
            two_bytes[0] = z;
        } else {
            two_bytes[1] = z;

            two_bytes_hex = (two_bytes[1] << 8) + two_bytes[0];
//            std::cout << std::hex << "two_bytes_hex -- " << two_bytes_hex << std::endl;
            if (two_bytes_hex < high_ten_bits_limit || (upper_limits_simple_symbols[0] < two_bytes_hex &&
                    upper_limits_simple_symbols[1] > two_bytes_hex)) {
                num_code_points++;
            }

        }
//        std::cout << "s[i] -- " << int(s[i]) << "  z -- " << z << ", ";

#ifdef DEBUG_MODE
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << z << " " << std::endl;
#endif
    }

#ifdef DEBUG_MODE
    std::cout << std::endl;
#endif

    return num_code_points;
}


size_t utf16_my_count_code_points_BE(const std::string& s, bool upper_case, int &skip_BOM_idx) {
    size_t num_code_points = 0;

#ifdef DEBUG_MODE
    std::cout << "Hex bytes of the file content" << std::endl;
#endif

    int two_bytes[2];
    int two_bytes_hex;

    // The low ten bits (also in the range 0x000–0x3FF) are added to 0xDC00 to give the second 16-bit code unit or
    // low surrogate (W2), which will be in the range 0xDC00–0xDFFF
    //
    // The high ten bits in range 0xD800–0xDBFF
    //
    // Another symbols in range U+0000 to U+D7FF and U+E000 to U+FFFF
    int high_ten_bits_limit = 0xdc00;
    int upper_limits_simple_symbols[2] = {0xe000, 0xffff};

    bool is_even = false;
    for (std::string::size_type i = s.length() - 1; i >= skip_BOM_idx; --i) {
        int z = s[i] & 0xff;

        // when for-cycle was started from 0 index, so
        // i % 2 == 0 is odd number
        // i % 2 == 1 is even number
        if (!is_even) {
            two_bytes[0] = z;
        } else {
            two_bytes[1] = z;

            two_bytes_hex = (two_bytes[1] << 8) + two_bytes[0];
//            std::cout << std::hex << "two_bytes_hex -- " << two_bytes_hex << std::endl;
            if (two_bytes_hex < high_ten_bits_limit || (upper_limits_simple_symbols[0] < two_bytes_hex &&
                    upper_limits_simple_symbols[1] > two_bytes_hex)) {
                num_code_points++;
            }

        }
        is_even = !is_even;
//        std::cout << "s[i] -- " << int(s[i]) << "  z -- " << z << ", ";

#ifdef DEBUG_MODE
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << z << " " << std::endl;
#endif
    }

#ifdef DEBUG_MODE
    std::cout << std::endl;
#endif

    return num_code_points;
}


size_t utf32_my_count_code_points(const std::string& s, bool upper_case, int &skip_BOM_idx) {

#ifdef DEBUG_MODE
    std::cout << "Hex bytes of the file content" << std::endl;

    for (std::string::size_type i = skip_BOM_idx; i < s.length(); ++i) {
        int z = s[i] & 0xff;

//        std::cout << "s[i] -- " << int(s[i]) << "  z -- " << z << ", ";

        std::cout << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << z << " ";
    }

    std::cout << std::endl;
#endif

    return (s.length() - skip_BOM_idx) / 4;
}
