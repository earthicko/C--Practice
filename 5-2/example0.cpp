#include <iostream>

class A {
   private:
    void private_func() { std::cout << "Private function of A" << std::endl; };
    int private_var;

    friend void func();
    friend class B;
};

class B {
   public:
    void friend_a() {
        A a;
        a.private_func();
        a.private_var = 1;
        std::cout << "Private var of A: " << a.private_var << std::endl;
    }
};

void func() {
    A a;
    a.private_func();
    a.private_var = 1;
    std::cout << "Private var of A: " << a.private_var << std::endl;
}

int main(){
    B b;
    b.friend_a();
    func();
}