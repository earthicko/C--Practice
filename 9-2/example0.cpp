#include <iostream>

template <typename T>
void print(T arg) {
    std::cout << arg << '\n';
}

template <typename T, typename... Types>
void print(T arg, Types... args) {
    std::cout << arg << ", ";
    print(args...);
}

int main() {
    print(3, 2, "abc", 5.3);
    print("def");

    return 0;
}