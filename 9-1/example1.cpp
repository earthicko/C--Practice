#include <iostream>
#include <string>

template <typename Type>
Type max(Type& a, Type& b) {
    return a > b ? a : b;
}

int main() {
    int a = 1, b = 2;
    std::cout << "Max (" << a << ", " << b << "): " << max(a, b) << std::endl;

    std::string s = "Hello", t = "World";
    std::cout << "Max (" << s << ", " << t << "): " << max(s, t) << std::endl;

    return 0;
}