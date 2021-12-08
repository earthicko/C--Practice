#include <iostream>

class Base {
   public:
    Base() { std::cout << "Base class" << std::endl; }

    virtual void describe() {
        std::cout << "describe() of base class" << std::endl;
    }
};

class Derived : public Base {
   public:
    Derived() : Base() { std::cout << "Derived class" << std::endl; }

    void describe() { std::cout << "describe() of derived class" << std::endl; }
};

int main() {
    Base base;
    Derived derived;

    Base* p_base = &base;
    Base* p_derived = &derived;

    std::cout << "==actual object is 'Base'==" << std::endl;
    p_base->describe();

    std::cout << "==actual object is 'Derived'==" << std::endl;
    p_derived->describe();

    return 0;
}