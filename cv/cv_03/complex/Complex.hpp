// next line is include guard, do not modify
#pragma once

// Complex class

// should have good ctors

// should have usable op+, op-, op+=, op-= and op<<
#include "iostream"

struct Complex {
public:
    Complex(double real, double img = 0);

    friend Complex operator+(const Complex &a, const Complex &b);

    friend Complex operator-(const Complex &a, const Complex &b);

    Complex &operator+=(const Complex &b);

    Complex &operator-=(const Complex &b);

    friend std::ostream &operator<<(std::ostream &out, const Complex &c);

private:
    double real, img;
};