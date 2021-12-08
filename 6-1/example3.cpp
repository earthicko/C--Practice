#include <iostream>

class Base {
    std::string parent_string;

   public:
    Base() : parent_string("기반") { std::cout << "기반 클라스" << std::endl; }

    void describe() { std::cout << parent_string << std::endl; }
};

class Derived : public Base {
    std::string child_string;

   public:
    Derived() : Base(), child_string("파생") {
        std::cout << "파생 클라스" << std::endl;
        describe();
    }

    void describe() { std::cout << child_string << std::endl; }
};

int main() {
    std::cout << "기반 클라스 생성!!" << std::endl;
    Base base;
    base.describe();

    std::cout << "파생 클라스 생성!!" << std::endl;
    Derived derived;
    return 0;
}