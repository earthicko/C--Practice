#include <math.h>

#include <iostream>
int char_len(const char* str);
double char_to_double(const char* str, int str_len);
int count_digits(double source);
class Complex {
   private:
    double real, img;

   public:
    Complex() : real(0), img(0) {}
    Complex(double real, double img) : real(real), img(img) {}
    Complex(double real) : real(real), img(0) {}
    Complex(const Complex& target) {
        real = target.real;
        img = target.img;
    }
    Complex(const char* s) {
        Complex c = parse_char(s);
        real = c.real;
        img = c.img;
    }

    Complex& operator=(const Complex& target);
    Complex& operator+=(const Complex& target);
    Complex& operator-=(const Complex& target);
    Complex& operator*=(const Complex& target);
    Complex& operator/=(const Complex& target);
    friend Complex operator+(const Complex& a, const Complex& b);
    friend Complex operator-(const Complex& a, const Complex& b);
    friend Complex operator*(const Complex& a, const Complex& b);
    friend Complex operator/(const Complex& a, const Complex& b);

    char* to_string() const;
    void print() const { std::cout << to_string() << "i)"; }
    void println() const {
        print();
        std::cout << std::endl;
    }
    friend std::ostream& operator<<(std::ostream& o, const Complex& out);
    static Complex parse_char(const char* str);
};

Complex& Complex::operator=(const Complex& target) {
    real = target.real;
    img = target.img;
    return *this;
}
Complex& Complex::operator+=(const Complex& target) {
    (*this) = (*this) + target;
    return *this;
}
Complex& Complex::operator-=(const Complex& target) {
    (*this) = (*this) - target;
    return *this;
}
Complex& Complex::operator*=(const Complex& target) {
    (*this) = (*this) * target;
    return *this;
}
Complex& Complex::operator/=(const Complex& target) {
    (*this) = (*this) / target;
    return *this;
}
Complex operator+(const Complex& a, const Complex& b) {
    Complex output(a.real + b.real, a.img + b.img);
    return output;
}
Complex operator-(const Complex& a, const Complex& b) {
    Complex output(a.real - b.real, a.img - b.img);
    return output;
}
Complex operator*(const Complex& a, const Complex& b) {
    double new_real = a.real * b.real - a.img * b.img;
    double new_img = a.img * b.real + a.real * b.img;
    Complex output(new_real, new_img);
    return output;
}
Complex operator/(const Complex& a, const Complex& b) {
    double com_div = b.real * b.real + b.img * b.img;
    double new_real = (a.real * b.real + a.img * b.img) / com_div;
    double new_img = (a.img * b.real - a.real * b.img) / com_div;
    Complex output(new_real, new_img);
    return output;
}
char* Complex::to_string() const {
    // 0123456789
    // (rrr + ii)
    // real_digits = 3;
    // img_digits = 2;
    // total_digits = 10;
    // real -> 3 2 1
    int real_digits = count_digits(real);
    int img_digits = count_digits(img);
    int real_c = real;
    int img_c = img;
    if (real_c < 0) {
        real_c = -real_c;
        real_digits++;
    }
    int total_digits = 5 + real_digits + img_digits;
    char* output = new char[total_digits];
    output[0] = '(';
    for (int i = real_digits; i >= 1; i--) {
        output[i] = (char)((real_c % 10) + 48);
        real_c /= 10;
    }
}
Complex Complex::parse_char(const char* str) {
    /*
    0 1 2 3
    7 . 9 i
    len = 4, real_pos = -1, img_pos = 3

    0 1 2 3 4
    - 3 + 8 i
    len = 5, real_pos = 2, img_pos = 4

    0 1 2 3 4 5 6
    1 2 - 9 6 7 i
    len = 7, real_pos = 2, img_pos = 6

    0 1 2 3 4 5 6 7
    1 . 5 + 2 . 8 i
    len = 8, real_pos = 3, img_pos = 7
    */
    double real;
    double img;

    int real_pos = -1;
    int& real_len = real_pos;
    int img_pos = -1;
    int img_len = -1;

    int len = char_len(str);

    if (str[len - 1] == 'i') {
        // img part present
        img_pos = len - 1;
        for (int i = 1; i < len - 1; i++)
            if (str[i] == '+' || str[i] == '-') {
                real_pos = i;
                break;
            }
        if (real_pos == -1) {
            // real part not present
            img_len = len - 1;
            char* img_str = new char[img_len];
            for (int i = 0; i < img_len; i++) img_str[i] = str[i];
            real = 0;
            img = char_to_double(img_str, img_len);
        } else {
            // real part present
            char* real_str = new char[real_len];
            for (int i = 0; i < real_len; i++) real_str[i] = str[i];
            img_len = img_pos - real_pos - 1;
            char* img_str = new char[img_len];
            for (int i = 0; i < img_len; i++)
                img_str[i] = str[i + real_pos + 1];
            real = char_to_double(real_str, real_len);
            img = char_to_double(img_str, img_len);
        }
    } else {
        // img part not present -> 0i
        real = char_to_double(str, len);
        img = 0;
    }

    Complex output(real, img);
    return output;
}
std::ostream& operator<<(std::ostream& os, const Complex& out) {
    os << out.to_string();
    return os;
}
int main() {
    // Complex a(1.0, 2.0);
    // Complex b(3.0, -2.0);
    // Complex c(0.0, 0.0);
    // c = a * b + a / b + a + b;
    // c.println();
    // c -= b;
    // c.println();
    // b.println();
    char test0[] = "3+2i";
    char test1[] = "-3+2i";
    char test2[] = "0098.7883-2.89i";
    char test3[] = "3001.3";
    char test4[] = "0023.040i";

    Complex target(2, 3);
    Complex res0 = target + test0;
    Complex res0_r = test0 + target;
    Complex res1 = target - test1;
    Complex res1_r = test1 - target;
    Complex res2 = target * test2;
    Complex res2_r = test2 * target;
    Complex res3 = target / test3;
    Complex res3_r = test3 / target;
    Complex res4 = target + test4;
    Complex res4_r = test4 + target;

    std::cout << res0 << std::endl;
    std::cout << res0_r << std::endl;
    std::cout << res1 << std::endl;
    std::cout << res1_r << std::endl;
    std::cout << res2 << std::endl;
    std::cout << res2_r << std::endl;
    std::cout << res3 << std::endl;
    std::cout << res3_r << std::endl;
    std::cout << res4 << std::endl;
    std::cout << res4_r << std::endl;

    res0 = res0_r + target;
    res1 = res1_r + target;
    res2 = res2_r + target;
    res3 = res3_r + target;
    res4 = res4_r + target;

    std::cout << res0 << std::endl;
    std::cout << res1 << std::endl;
    std::cout << res2 << std::endl;
    std::cout << res3 << std::endl;
    std::cout << res4 << std::endl;

    return 0;
}
int char_len(const char* str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}
double char_to_double(const char* str, int str_len) {
    /*
    0 1 2 3 4 5 6
    - 4 8 7 . 5 9
    */
    bool sign = true;
    if (str[0] == '-') sign = false;
    double output = 0;
    int decimal_pos = str_len;
    for (int i = 0; i < str_len; i++) {
        if (str[i] == '.') {
            decimal_pos = i;
            break;
        }
    }
    for (int i = decimal_pos + 1; i < str_len; i++) {
        int num = str[i] - 48;
        int power = i - decimal_pos;
        output += num * pow(0.1, power);
    }
    for (int i = decimal_pos - 1; i >= 0; i--) {
        if (str[i] == '-' || str[i] == '+') continue;
        int num = str[i] - 48;
        int power = decimal_pos - i - 1;
        output += num * pow(10, power);
    }
    if (sign == false) output = -output;
    return output;
}
int count_digits(double source) {
    int digits = 0;
    while (source != 0) {
        digits++;
        source /= 10;
    }
    return digits;
}