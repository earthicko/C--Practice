#include "vector.h"

#include <iostream>
namespace MyExcel {
Vector::Vector(int n) : data(new std::string[n]), capacity(n), length(0) {}

Vector::Vector(const Vector& src) : capacity(src.capacity), length(src.length) {
    data = new std::string[capacity];
    for (int i = 0; i < capacity; i++) data[i] = src.data[i];
}

void Vector::push_back(std::string s) {
    if (capacity <= length) {
        std::string* temp = data;
        data = new std::string[capacity * 2];
        for (int i = 0; i < length; i++) data[i] = temp[i];
        delete[] temp;
        capacity *= 2;
    }

    data[length] = s;
    length++;
}

std::string Vector::operator[](int i) {
    if (0 <= i && i < length)
        return data[i];
    else
        return "";
}

void Vector::remove(int index) {
    if (0 <= index && index < length) {
        /*
        a b c d e
        0 1 2 3 4
        remove 2
        3->2, 4->3
        */
        for (int i = index + 1; i < length; i++) data[i - 1] = data[i];
        length--;
    }
}

int Vector::size() { return length; }

void Vector::print_all() const {
    for (int i = 0; i < length; i++) std::cout << data[i] << " ";
    std::cout << '\n';
}

Vector::~Vector() {
    if (data) {
        delete[] data;
    }
}
}  // namespace MyExcel