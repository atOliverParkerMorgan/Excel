#include "Complex.hpp"


Complex::Complex(double _real, double _img) : real(_real), img(_img) {}

Complex operator+(const Complex &a, const Complex &b) {
    return {a.real + b.real, a.img + b.img};
}

Complex operator-(const Complex &a, const Complex &b) {
    return {a.real - b.real, a.img - b.img};
}

Complex &Complex::operator+=(const Complex &b) {
    this->real += b.real;
    this->img += b.img;
    return *this;
}

Complex &Complex::operator-=(const Complex &b) {
    return *this = *this - b;
}

std::ostream &operator<<(std::ostream &out, const Complex &c) {
    return out << "[" << c.real << ", " << c.img << "]";
}