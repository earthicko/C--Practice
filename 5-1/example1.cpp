#include <iostream>

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
int main() {
    Complex a(1.0, 2.0);
    Complex b(3.0, -2.0);
    Complex c(0.0, 0.0);
    c = a * b + a / b + a + b;
    c.println();
    c -= b;
    c.println();
    b.println();
    return 0;
}