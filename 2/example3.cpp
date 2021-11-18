#include <iostream>

int& function(int& a) {
    a = 5;
    return a;
}

int main() {
    int b = 2;
    std::cout << b << std::endl;
    int c = function(b);
    std::cout << c << std::endl;
    return 0;
}