#include "stack.h"

#include <iostream>

namespace MyExcel {
StrStack::StrStack() { head = NULL; }

StrStack::StrStack(const std::string& init) {
    head = new struct node;
    head->next = NULL;
    head->data = init;
}

void StrStack::push(const std::string& new_data) {
    struct node* new_node = new struct node;
    new_node->next = head;
    new_node->data = new_data;
    head = new_node;
}

std::string StrStack::pop() {
    if (is_empty()) return "";
    std::string popped = head->data;
    struct node* temp = head;
    head = head->next;
    delete temp;
    return popped;
}

std::string StrStack::peek() const {
    if (is_empty())
        return "";
    else
        return head->data;
}

bool StrStack::is_empty() const {
    if (head == NULL)
        return true;
    else
        return false;
}

StrStack::~StrStack() {
    struct node* target = head;
    while (target) {
        struct node* next = target->next;
        delete target;
        target = next;
    }
}

NumStack::NumStack() { head = NULL; }

NumStack::NumStack(const float& init) {
    head = new struct node;
    head->next = NULL;
    head->data = init;
}

void NumStack::push(const float& new_data) {
    struct node* new_node = new struct node;
    new_node->next = head;
    new_node->data = new_data;
    head = new_node;
}

float NumStack::pop() {
    if (is_empty()) return 0.0;
    float popped = head->data;
    struct node* temp = head;
    head = head->next;
    delete temp;
    return popped;
}

float NumStack::peek() const {
    if (is_empty())
        return 0.0;
    else
        return head->data;
}

bool NumStack::is_empty() const {
    if (head == NULL)
        return true;
    else
        return false;
}

void NumStack::print_all() const {
    struct node* target = head;
    while (target) {
        std::cout << target->data << " ";
        target = target->next;
    }
    std::cout << '\n';
}

NumStack::~NumStack() {
    struct node* target = head;
    while (target) {
        struct node* next = target->next;
        delete target;
        target = next;
    }
}
}  // namespace MyExcel