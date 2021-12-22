#include <string.h>

#include <iostream>
#include <string>

template <typename Str>
void append_to_str(Str* orig) {
    return;
}

template <typename Orig_str, typename Str, typename... Strs>
void append_to_str(Orig_str* orig, const Str& appended, Strs... strs) {
    orig->append(appended);
    append_to_str(orig, strs...);
}

size_t get_str_size(const char* s) { return strlen(s); }
size_t get_str_size(const std::string& s) { return s.size(); }

template <typename Str, typename... Strs>
size_t get_str_size(const Str& str, Strs... strs) {
    return get_str_size(str) + get_str_size(strs...);
}

template <typename Str, typename... Strs>
std::string concat_str(const Str& str, Strs... strs) {
    size_t total_size = get_str_size(str, strs...);

    std::string output;
    output.reserve(total_size);
    output = str;

    append_to_str(&output, strs...);

    return output;
}

int main() {
    std::cout << concat_str("Hell", "o", std::string(", World"), "!");
}