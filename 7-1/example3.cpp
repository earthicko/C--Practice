#include <iostream>
#include <string>

int main() {
    std::string s;

    std::cin >> s;

    std::cout << "Read first: " << s << std::endl;

    char peek = std::cin.rdbuf()->snextc();
    if (std::cin.fail()) std::cout << "Peeking failed" << std::endl;

    std::cout << "First char of the second word: " << peek << std::endl;
    std::cin >> s;
    std::cout << "Read again: " << s << std::endl;

    return 0;
}