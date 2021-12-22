#include <iostream>

int sum_all() { return 0; }

template <typename... Ints>
int sum_all(int base, Ints... ints) {
    return base + sum_all(ints...);
}

template <typename... Ints>
double average(Ints... ints) {
    return static_cast<double>(sum_all(ints...)) / sizeof...(ints);
}

int main() {
    std::cout << average(2, 3, 1, 0, 28) << '\n';

    return 0;
}