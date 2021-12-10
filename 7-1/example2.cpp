#include <iostream>
#include <string>

int main() {
    int t;
    std::string s;

    while (true) {
        std::cin >> std::hex >> t;

        if (std::cin.fail()) {
            std::cout << "Incorrect input value" << std::endl;
            std::cin.clear();
            std::cin.ignore(100, '\n');
            continue;
        }

        std::cout << "Input: " << t << std::endl;
        if (t == 0) break;
    }

    return 0;
}