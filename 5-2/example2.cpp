#include <iostream>

int char_len(const char* str);

class String {
    char* str;
    int len;
    int max_len;

   public:
    String();
    String(char c);
    String(char c, int n);
    String(const char* s);
    String(const String& s);
    ~String() {
        if (str != NULL) delete[] str;
    }

    String& assign_null();
    String& assign(const String& original);
    String& assign(const char* original);

    void add_string(const String& s);
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
    String& erase(int loc, int num);

    static bool is_identical(const String& a, const String& b);
    static int compare(const String& a, const String& b);

    int find(int find_from, const String& target) const;
    int find(int find_from, const char* target) const;
    int find(int find_from, char c) const;

    char& operator[](const int index);

    bool operator==(String& target);
    friend std::ostream& operator<<(std::ostream& out, const String& string);
};
String::String() {
    len = 0;
    max_len = 0;
    str = NULL;
}
String::String(char c) {
    len = 1;
    max_len = 2;
    str = new char[max_len];
    str[0] = c;
}
String::String(char c, int n) {
    if (n > 0) {
        len = n;
        max_len = 2 * n;
        str = new char[max_len];
        for (int i = 0; i != n; i++) str[i] = c;
    } else {
        len = 0;
        max_len = 0;
        str = NULL;
    }
}
String::String(const char* s) {
    int i = 0;
    do i++;
    while (s[i]);
    len = i;
    max_len = 2 * i;
    if (len > 0) {
        str = new char[max_len];
        for (int i = 0; i != len; i++) str[i] = s[i];
    } else
        str = NULL;
}
String::String(const String& s) {
    len = s.len;
    max_len = len * 2;
    if (len > 0) {
        str = new char[max_len];
        for (int i = 0; i != len; i++) str[i] = s.str[i];
    } else
        str = NULL;
}
String& String::assign_null() {
    len = 0;
    max_len = 0;
    if (str != NULL) {
        delete[] str;
        str = NULL;
    }
    return *this;
}
String& String::assign(const String& source) {
    if (source.str == NULL) {
        assign_null();
        return *this;
    }
    if (max_len < source.len) reserve(source.len * 2);
    len = source.len;
    for (int i = 0; i != len; i++) str[i] = source.str[i];
    return *this;
}
String& String::assign(const char* source) {
    if (source == NULL) {
        assign_null();
        return *this;
    }
    int new_len = char_len(source);
    if (max_len < new_len) reserve(new_len * 2);
    len = new_len;
    for (int i = 0; i != len; i++) str[i] = source[i];
    return *this;
}
void String::add_string(const String& s) {
    int new_len = len + s.len;
    if (max_len < new_len) reserve(new_len * 2);
    for (int i = 0; i != s.len; i++) str[i + len] = s.str[i];
    len = new_len;
}
void String::print() const {
    for (int i = 0; i != len; i++) std::cout << str[i];
}
void String::println() const {
    print();
    std::cout << std::endl;
}
std::ostream& operator<<(std::ostream& out, const String& string) {
    for (int i = 0; i < string.len; i++) out << string.str[i];
    return out;
}
void String::describe() const {
    std::cout << "Content: ";
    println();
    std::cout << "len: " << len << " max len: " << max_len << std::endl;
}
String String::get_subtring(int start, int end) {
    /*
    0 1 2 3 4 5 6 7
    a b c d e f g h

    get_substring(2, 6) => cdef
    new_len = 6 - 2;
    2, 3, 4, 5: copy

    0 1 2 3
    c d e f
    */
    int new_len = end - start;
    if (start < 0 || end < 0 || start >= len || end >= len || new_len <= 0) {
        String output("\0");
        output.assign_null();
        return output;
    } else {
        char* new_str = new char[new_len];
        for (int i = 0; i != new_len; i++) new_str[i] = str[i + start];
        String output(new_str);
        return output;
    }
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
        for (int i = 0; i != len; i++) str[i] = prev_str[i];
        max_len = new_cap;
        delete[] prev_str;
    }
}
String& String::insert(int loc, const String& source) {
    if (loc < 0 || loc >= len) return *this;
    int new_len = len + source.len;
    if (max_len < new_len) {
        int new_max_len = new_len * 2;
        char* prev_str = str;
        str = new char[new_max_len];
        for (int i = 0; i != loc; i++) str[i] = prev_str[i];
        for (int i = 0; i != source.len; i++) str[i + loc] = source.str[i];
        for (int i = loc; i != len; i++) str[i + source.len] = prev_str[i];
        max_len = new_max_len;
        len = new_len;
        delete[] prev_str;
    } else {
        for (int i = len - 1; i != loc - 1; i--) str[i + source.len] = str[i];
        for (int i = loc; i != loc + source.len; i++)
            str[i] = source.str[i - loc];
        len = new_len;
    }
    return *this;
}
String& String::insert(int loc, const char* source) {
    if (loc < 0 || loc >= len) return *this;
    int source_len = char_len(source);
    int new_len = len + source_len;
    if (max_len < new_len) {
        int new_max_len = new_len * 2;
        char* prev_str = str;
        str = new char[new_max_len];
        for (int i = 0; i != loc; i++) str[i] = prev_str[i];
        for (int i = 0; i != source_len; i++) str[i + loc] = source[i];
        for (int i = loc; i != len; i++) str[i + source_len] = prev_str[i];
        max_len = new_max_len;
        len = new_len;
        delete[] prev_str;
    } else {
        for (int i = len - 1; i != loc - 1; i--) {
            str[i + source_len] = str[i];
        }
        for (int i = loc; i != loc + source_len; i++) {
            str[i] = source[i - loc];
        }
        len = new_len;
    }
    return *this;
}
String& String::insert(int loc, char c) {
    if (loc < 0 || loc >= len) return *this;
    if (max_len < len + 1) {
        int new_max_len = (len + 1) * 2;
        char* prev_str = str;
        str = new char[new_max_len];
        for (int i = 0; i != loc; i++) str[i] = prev_str[i];
        str[loc] = c;
        for (int i = loc + 1; i != len + 1; i++) str[i] = prev_str[i - 1];
        delete[] prev_str;
        max_len = new_max_len;
        len++;
    } else {
        for (int i = len - 1; i != loc - 1; i--) str[i + 1] = str[i];
        str[loc] = c;
        len++;
    }
    return *this;
}
String& String::erase(int loc, int num) {
    /*
    0 1 2 3 4 5 6 7
    a b c d e f g h
    loc=2, num=3;
    5->2, 6->3, 7->4
    len -= num;
    */
    if (num <= 0 || loc < 0 || loc >= len) return *this;
    if (loc + num > len) return *this;
    for (int i = loc + num; i < len; i++) str[i - num] = str[i];
    len -= num;
    return *this;
}
bool String::is_identical(const String& a, const String& b) {
    if (a.len != b.len) return false;
    for (int i = 0; i != a.len; i++)
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
    for (int i = 0; i != min_len; i++) {
        if (a.str[i] < b.str[i])
            return 1;
        else if (a.str[i] > b.str[i])
            return -1;
    }
    return 0;
}
int String::find(int find_from, const String& target) const {
    /*
    0 1 2 3 4 5 6 7
    a b c d e f g h

    str=fgh, find_from=6 -> find_from + str_len > len -> return -1;
    */
    if (find_from < 0 || find_from >= len || find_from + target.len > len)
        return -1;
    for (int i = find_from; i <= len - target.len; i++) {
        int j = 0;
        for (j = 0; j < target.len; j++)
            if (str[j + i] != target.str[j]) break;
        if (j == target.len) return i;
    }
    return -1;
}
int String::find(int find_from, const char* target) const {
    int target_len = char_len(str);
    if (find_from < 0 || find_from >= len || find_from + target_len > len)
        return -1;
    for (int i = find_from; i <= len - target_len; i++) {
        int j = 0;
        for (j = 0; j < target_len; j++)
            if (str[j + i] != target[j]) break;
        if (j == target_len) return i;
    }
    return -1;
}
int String::find(int find_from, char c) const {
    if (find_from < 0 || find_from >= len) return -1;
    for (int i = find_from; i < len; i++) {
        if (str[i] == c) return i;
    }
    return -1;
}
char& String::operator[](const int index) { return str[index]; }
bool String::operator==(String& target) { return is_identical(*this, target); }
int main() {
    String apples[10];
    for (int i = 0; i != 10; i++) apples[i].assign("apple");
    String microsoft("-microsoft-");
    apples[0].describe();
    apples[0].reserve(30);
    apples[0].describe();
    apples[0].insert(3, microsoft);
    apples[0].describe();

    apples[1].insert(2, '-');
    apples[1].describe();
    apples[2].insert(4, "-d-");
    apples[2].describe();
    apples[3].insert(6, microsoft);
    apples[3].describe();

    String substring = apples[0].get_subtring(4, 8);
    std::cout << substring << std::endl;

    apples[4].assign_null();
    std::cout << apples[4] << std::endl;

    apples[5].reserve(30);
    std::cout << "apple vs apple: ";
    if (apples[5] == apples[6])
        std::cout << "identical" << std::endl;
    else
        std::cout << "not identical" << std::endl;

    std::cout << "apple vs ap-ple: ";
    if (apples[5] == apples[1])
        std::cout << "identical" << std::endl;
    else
        std::cout << "not identical" << std::endl;

    apples[6].erase(1, 2);
    apples[6].describe();
    apples[7].erase(1, 1000);
    apples[7].describe();

    String target("ple");
    String too_long_target("plee");
    String non_existence_target("fuck");
    std::cout << "find ple from apple: " << apples[7].find(0, target)
              << std::endl;
    std::cout << "find plee from apple: " << apples[7].find(0, too_long_target)
              << std::endl;
    std::cout << "find fuck from apple: "
              << apples[7].find(0, non_existence_target) << std::endl;
    std::cout << "find p from apple from 0: " << apples[7].find(0, 'p')
              << std::endl;
    std::cout << "find p from apple from 2: " << apples[7].find(2, 'p')
              << std::endl;

    std::cout << 3 << "th index of " << apples[7] << ": " << apples[7][3]
              << std::endl;

    return 0;
}
int char_len(const char* str) {
    // 0 1 2 3 4
    // a j d f 0
    int i = 0;
    while (str[i]) i++;
    return i;
}