#include <iostream>
namespace MyExcel {
class StrStack {
    struct node {
        struct node* next;
        std::string data;
    };
    struct node* head;

   public:
    StrStack();
    StrStack(const std::string& init);

    void push(const std::string& new_data);
    std::string pop();
    std::string peek() const;
    bool is_empty() const;

    ~StrStack();
};

class NumStack {
    struct node {
        struct node* next;
        float data;
    };
    struct node* head;

   public:
    NumStack();
    NumStack(const float& init);

    void push(const float& new_data);
    float pop();
    float peek() const;
    bool is_empty() const;

    ~NumStack();
};
}  // namespace MyExcel