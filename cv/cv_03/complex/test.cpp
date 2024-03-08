#ifndef __TRAINER__

#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <tuple>
#include <iomanip>
#include <cassert>
#include "Complex.hpp"

int main() {
    Complex a(3, 4), b(5, 6);

    {
        std::ostringstream os;
        Complex c(1.5, 2.5);
        os << c;
        assert(os.str() == "[1.5, 2.5]");
    }

    // +
    {
        std::ostringstream os;
        Complex c = a + b;
        Complex d = c + 3;
        Complex e = 7 + d;
        Complex f = Complex(1, 2) + Complex(11, 12);
        os << f + e;
        assert(os.str() == "[30, 24]");
    }
    // -
    {
        std::ostringstream os;
        Complex c = a - b;
        Complex d = c - 3;
        Complex e = 7 - d;
        Complex f = Complex(1, 2) - Complex(11, 12);
        os << f - e;
        assert(os.str() == "[-22, -12]");
    }

    // +=
    {
        std::ostringstream os;
        Complex c = a;
        c += Complex(3, 5);
        (c += 7) += Complex(12, 11);
        os << c;
        assert(os.str() == "[25, 20]");
    }

    // -=
    {
        std::ostringstream os;
        Complex c = a;
        c -= Complex(3, 5);
        (c -= 7) -= Complex(12, 11);
        os << c;
        assert(os.str() == "[-19, -12]");
    }

    // <<
    {
        std::ostringstream os;
        os << Complex(3) << Complex(2, 5) << a + b << a - b;
        assert(os.str() == "[3, 0][2, 5][8, 10][-2, -2]");
    }
}

#endif