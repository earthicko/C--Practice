#include <iostream>

int main() {
    int a, b;
    a = 0;
    b = 1;
    int arr[2] = {a, b};
    // int& illegal_arr[2] = {a, b};
    int(&legal_arr)[2] = arr;
    std::cout << "a: " << arr[0] << " b:" << arr[1] << std::endl;

    legal_arr[0] = 10;
    legal_arr[1] = 11;
    std::cout << "a: " << arr[0] << " b:" << arr[1] << std::endl;

    return 0;
}