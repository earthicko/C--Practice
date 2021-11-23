#include <iostream>

int char_len(const char* str);

class String {
    char* str;
    int len;
    int max_len;

   public:
    String(char c, int n);
    String(const char* s);
    String(const String& s);
    ~String() {
        this->print();
        std::cout << ": deleted" << std::endl;
        delete[] str;
    }

    String& assign(const String& original);
    String& assign(const char* original);

    void add_string(const String& s);
    void copy_string(const String& s);
    void print() const;
    void println() const;
    void describe() const;
    String get_subtring(int start, int end);
    char at(int i) const;
    int strlen() const;
    int capacity() const;
    void reserve(int new_cap);

    String& insert(int loc, const String& str);
    String& insert(int loc, const char* str);
    String& insert(int loc, char c);

    static bool is_identical(const String& a, const String& b);
    static int compare(const String& a, const String& b);
};

String::String(char c, int n) {
    str = new char[n];
    for (int i = 0; i < n; i++) {
        str[i] = c;
    }
    len = n;
}

String::String(const char* s) {
    int i = 0;
    while (1) {
        i++;
        if (s[i] == (char)NULL) break;
    }
    len = i;
    max_len = i;
    str = new char[len];
    for (int i = 0; i < len; i++) {
        str[i] = s[i];
    }
}

String::String(const String& s) {
    len = s.len;
    max_len = s.len;
    str = new char[len + 1];
    for (int i = 0; i < len + 1; i++) {
        str[i] = s.str[i];
    }
}

String& String::assign(const String& original) {
    if (len < original.len) {
        delete[] str;
        max_len = original.len;
        str = new char[len];
    }

    len = original.len;
    for (int i = 0; i < len; i++) {
        str[i] = original.str[i];
    }
    return *this;
}

String& String::assign(const char* original) {
    String temp(original);
    len = temp.len;
    delete[] str;
    str = new char[len];
    for (int i = 0; i < len; i++) {
        str[i] = original[i];
    }
    return *this;
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

void String::print() const {
    for (int i = 0; i < len; i++) std::cout << str[i];
}
void String::println() const {
    this->print();
    std::cout << std::endl;
}

void String::describe() const {
    std::cout << "Content: ";
    this->println();
    std::cout << "len: " << len << " max len: " << max_len << std::endl;
}

String String::get_subtring(int start, int end) {
    /*
    0 1 2 3 4 5 6 7 8
    a b c d e f g h 0

    get_substring(2, 6) => cdef
    new_len = 6 - 2;
    2, 3, 4, 5: copy
    6: NULL

    0 1 2 3 4
    c d e f 0
    */
    int new_len = end - start;
    char new_str[100];
    for (int i = 0; i < new_len; i++) {
        new_str[i] = str[i + start];
    }
    new_str[new_len] = (char)NULL;
    String output(new_str);
    return output;
}

char String::at(int i) const {
    if (i >= len || i < 0)
        return (char)NULL;
    else
        return str[i];
}

int String::strlen() const { return len; }

int String::capacity() const { return max_len; }
void String::reserve(int new_cap) {
    if (new_cap > max_len) {
        char* prev_str = str;
        str = new char[new_cap];
        for (int i = 0; i < len; i++) {
            str[i] = prev_str[i];
        }
        max_len = new_cap;
        delete[] prev_str;
    }
}
String& String::insert(int loc, const String& new_str) {
    if (max_len < len + new_str.len) {
        char* prev_str = str;
        str = new char[len + new_str.len];
        for (int i = 0; i != loc; i++) str[i] = prev_str[i];
        for (int i = 0; i != new_str.len; i++) str[i + loc] = new_str.str[i];
        for (int i = loc; i != len; i++) str[i + new_str.len] = prev_str[i];
        max_len = len + new_str.len;
        len = max_len;
        delete[] prev_str;
    } else {
        for (int i = len - 1; i != loc; i--) str[i + new_str.len] = str[i];
        for (int i = loc; i != loc + new_str.len; i++)
            str[i] = new_str.str[i - loc];
    }
    return *this;
}

String& String::insert(int loc, const char* new_str) {
    int new_str_len = char_len(new_str);
    if (max_len < len + new_str_len) {
        char* prev_str = str;
        str = new char[len + new_str_len];
        for (int i = 0; i != loc; i++) str[i] = prev_str[i];
        for (int i = 0; i != new_str_len; i++) str[i + loc] = new_str[i];
        for (int i = loc; i != len; i++) str[i + new_str_len] = prev_str[i];
        max_len = len + new_str_len;
        len = max_len;
        delete[] prev_str;
    } else {
        for (int i = len - 1; i != loc; i--) str[i + new_str_len] = str[i];
        for (int i = loc; i != loc + new_str_len; i++)
            str[i] = new_str[i - loc];
        len += new_str_len;
    }
    return *this;
}

String& String::insert(int loc, char c) {
    if (max_len < len + 1) {
        char* prev_str = str;
        str = new char[max_len + 1];
        for (int i = 0; i < loc; i++) str[i] = prev_str[i];
        str[loc] = c;
        for (int i = loc + 1; i < max_len + 1; i++) str[i] = prev_str[i - 1];
        delete[] prev_str;
        max_len++;
        len = max_len;
    } else {
        for (int i = len - 1; i >= loc; i--) str[i + 1] = str[i];
        str[loc] = c;
        len++;
    }
    return *this;
}

bool String::is_identical(const String& a, const String& b) {
    if (a.len != b.len) return false;
    for (int i = 0; i < a.len; i++)
        if (a.str[i] != b.str[i]) return false;
    return true;
}

int String::compare(const String& a, const String& b) {
    // 1 if a -> b, -1 if b -> a, 0 if a == b
    int min_len;
    if (a.len < b.len)
        min_len = a.len;
    else
        min_len = b.len;

    for (int i = 0; i < min_len; i++) {
        if (a.str[i] < b.str[i])
            return 1;
        else if (a.str[i] > b.str[i])
            return -1;
    }
    return 0;
}

int main() {
    String apple("apple");
    String microsoft("microsoft");
    apple.describe();
    apple.reserve(10);
    apple.describe();

    apple.insert(2, 'f');
    apple.describe();
    apple.insert(4, "de");
    apple.describe();
    apple.insert(6, microsoft);
    apple.describe();

    return 0;
}

int char_len(const char* str) {
    // 0 1 2 3 4
    // a j d f 0
    int i = 0;
    while (str[i]) i++;
    return i;
}