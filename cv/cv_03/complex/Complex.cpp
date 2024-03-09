#include "Complex.hpp"
#include <iostream>


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

Complex operator*(const Complex &a, const Complex &b) {
    return {a.real * b.real - a.img * b.img, a.real * b.img + a.img * b.real};
}

Complex operator/(const Complex &a, const Complex &b) {
    return {(a.real * b.real + a.img * b.img) / (a.img * a.img + b.img * b.img),
            (a.img * b.real - a.real * b.img) / (a.img * a.img + b.img * b.img)};
}

Complex &Complex::operator/=(const Complex &b) {
    real = (real * b.real + img * b.img) / (img * img + b.img * b.img);
    img = (img * b.real - real * b.img) / (img * img + b.img * b.img);
    return *this;
}

Complex &Complex::operator*=(const Complex &b) {
    real = real * b.real - img * b.img;
    img = real * b.img + img * b.real;
    return *this;
}

Complex operator-(const Complex &b) {
    return {-b.real, -b.img};
}

std::istream &operator>>(std::istream &in, Complex &c) {
    char h;
    double realPart, imgPart;
    in.clear();
    if(in >> h && h == '[' && in >> realPart  && in >> h && h==',' && in >> imgPart && in >> h && h == ']'){
        c.real = realPart;
        c.img = imgPart;
    }else{
        in.setstate(std::ios::failbit);
    }

    return in;
}
