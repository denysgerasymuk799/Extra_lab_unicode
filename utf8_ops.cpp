#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>


using namespace std;

string ToHex(const string& s, bool upper_case) {
    ostringstream ret;

    for (string::size_type i = 0; i < s.length(); ++i) {
        int z = s[i] & 0xff;
        ret << std::hex << std::setfill('0') << std::setw(2) << (upper_case ? std::uppercase : std::nouppercase) << z << " ";
    }

    return ret.str();
}


int main() {
    ifstream::pos_type size;
    char *memblock;

    string toread = "../test.txt";
    ifstream file(toread, ios::in | ios::binary | ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char[size];

        // TODO: test
        // skip BOM
        file.seekg(2, ios::beg);
        size -= 2;

        file.read (memblock, size);
        file.close();

        cout << "the complete file content is in memory" << endl;

        std::string tohexed = ToHex(memblock, true);


        std::cout << tohexed << std::endl;
    }
}