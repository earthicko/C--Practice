#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream in("./7-2/test");
    std::string s;

    if (in.is_open()) {
        in >> s;
        std::cout << "Read string: " << s << std::endl;
    } else {
        std::cout << "File not found" << std::endl;
    }

    return 0;
}