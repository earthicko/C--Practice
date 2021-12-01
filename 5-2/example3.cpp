#include <iostream>

class Int {
    int data;

   public:
    Int(int data) : data(data) {}
    Int(const Int& data) : data(data.data) {}
    operator int() { return data; }
};

int main() {
    Int a = 3;
    int b = 5;

    int c = a + b;
    Int d = a - b;

    std::cout << "a: " << a << std::endl;
    std::cout << "b: " << b << std::endl;
    std::cout << "c: " << c << std::endl;
    std::cout << "d: " << d << std::endl;

    return 0;
}