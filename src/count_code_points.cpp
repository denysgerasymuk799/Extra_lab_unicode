#include "../inc/count_code_points.hpp"


// returns the number of utf8 code points in the buffer at s
size_t utf8_my_count_code_points(char *s) {
    size_t len = 0;
    for (; *s; ++s) if ((*s & 0xC0) != 0x80) ++len;
    return len;
}


size_t utf8_count_code_points(std::string input) {
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


size_t utf16_my_count_code_points(const std::string& s, bool upper_case) {
//    std::ostringstream ret;
    size_t len = 0;

    for (std::string::size_type i = 0; i < s.length(); ++i) {
        int z = s[i] & 0xff;
#ifdef DEBUG_MODE
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << z << " ";
#endif
        len++;
    }

    return len;
}
