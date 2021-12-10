#include <iostream>

int main() {
    int t;

    while (std::cin >> t) {
        std::cout << "Input: " << t << std::endl;
        if (t == 0) break;
    }
    return 0;
}