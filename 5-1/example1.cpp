#include <math.h>

#include <iostream>
int char_len(const char* str);
double char_to_double(const char* str, int str_len);
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

    Complex operator+(const Complex& target);
    Complex operator-(const Complex& target);
    Complex operator*(const Complex& target);
    Complex operator/(const Complex& target);
    Complex& operator=(const Complex& target);
    Complex& operator+=(const Complex& target);
    Complex& operator-=(const Complex& target);
    Complex& operator*=(const Complex& target);
    Complex& operator/=(const Complex& target);

    void print() { std::cout << "(" << real << " + " << img << "i)"; }
    void println() {
        print();
        std::cout << std::endl;
    }
    static Complex parse_char(const char* str);
};

Complex Complex::operator+(const Complex& target) {
    Complex output(real + target.real, img + target.img);
    return output;
}
Complex Complex::operator-(const Complex& target) {
    Complex output(real - target.real, img - target.img);
    return output;
}
Complex Complex::operator*(const Complex& target) {
    double new_real = real * target.real - img * target.img;
    double new_img = img * target.real + real * target.img;
    Complex output(new_real, new_img);
    return output;
}
Complex Complex::operator/(const Complex& target) {
    double com_div = target.real * target.real + target.img * target.img;
    double new_real = (real * target.real + img * target.img) / com_div;
    double new_img = (img * target.real - real * target.img) / com_div;
    Complex output(new_real, new_img);
    return output;
}
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
    Complex res0 = Complex::parse_char(test0);
    Complex res1 = Complex::parse_char(test1);
    Complex res2 = Complex::parse_char(test2);
    Complex res3 = Complex::parse_char(test3);
    Complex res4 = Complex::parse_char(test4);
    res0.println();
    res1.println();
    res2.println();
    res3.println();
    res4.println();
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