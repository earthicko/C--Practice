#include <iostream>
namespace MyExcel {
class Vector {
    std::string* data;
    int capacity;
    int length;

   public:
    Vector(int n = 1);

    void push_back(std::string s);
    std::string operator[](int i);
    void remove(int i);
    int size();
    void print_all() const;

    ~Vector();
};
}  // namespace MyExcel