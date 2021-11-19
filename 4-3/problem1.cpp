#include <iostream>

class String {
    char* str;
    int len;

   public:
    String(char c, int n);
    String(const char* s);
    String(const String& s);

    void add_string(const String& s);
    void copy_string(const String& s);
    void print();
    int strlen();
};

String::String(char c, int n) {
    str = new char[n + 1];
    for (int i = 0; i < n; i++) {
        str[i] = c;
    }
    str[n] = (char)NULL;
    len = n;
}

String::String(const char* s) {
    int i = 0;
    while (1) {
        i++;
        if (s[i] == (char)NULL) break;
    }
    len = i;
    str = new char[len + 1];
    for (int i = 0; i < len + 1; i++) {
        str[i] = s[i];
    }
}

String::String(const String& s) {
    len = s.len;
    str = new char[len + 1];
    for (int i = 0; i < len + 1; i++) {
        str[i] = s.str[i];
    }
}

void String::add_string(const String& s) {
    int new_len = len + s.len;
    char* new_str = new char[new_len + 1];

    for (int i = 0; i < len; i++) new_str[i] = str[i];
    for (int i = len; i < new_len; i++) new_str[i] = s.str[i - len];
    str = new_str;
    len = new_len;
}

void String::copy_string(const String& s) {
    delete[] str;
    len = s.len;
    str = new char[len + 1];
    for (int i = 0; i < len + 1; i++) str[i] = s.str[i];
}

void String::print() { std::cout << str; }

int String::strlen() { return len; }

int main() {
    String repeated = String('d', 4);
    String const_char_p = String("FUck");
    String copied = String(repeated);
    String copied_2 = const_char_p;
    String added = repeated;
    added.add_string(const_char_p);
    String copied_after = repeated;
    copied_after.copy_string(const_char_p);

    repeated.print();
    std::cout << std::endl;
    const_char_p.print();
    std::cout << std::endl;
    copied.print();
    std::cout << std::endl;
    copied_2.print();
    std::cout << std::endl;
    added.print();
    std::cout << std::endl;
    copied_after.print();
    std::cout << std::endl;

    std::cout << "STRLEN: " << repeated.strlen() << std::endl;
    std::cout << "STRLEN: " << const_char_p.strlen() << std::endl;
    std::cout << "STRLEN: " << copied.strlen() << std::endl;
    std::cout << "STRLEN: " << copied_2.strlen() << std::endl;
    std::cout << "STRLEN: " << added.strlen() << std::endl;
    std::cout << "STRLEN: " << copied_after.strlen() << std::endl;

    return 0;
}