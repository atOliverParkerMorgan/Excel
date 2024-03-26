#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <compare>
#include <stdexcept>

class CBigInt {
public:
    // default constructor
    CBigInt();

    ~CBigInt() = default;
    // copying/assignment/destruction

    // int constructor
    CBigInt(int value);

    // string constructor
    CBigInt(const char in_value[]);

    // operator +, any combination {CBigInt/int/string} + {CBigInt/int/string}
    friend CBigInt operator+(const CBigInt &val1, const CBigInt &val2);

    // operator *, any combination {CBigInt/int/string} * {CBigInt/int/string}
    friend CBigInt operator*(const CBigInt &val1, const CBigInt &val2);

    // operator +=, any of {CBigInt/int/string}
    CBigInt &operator+=(const CBigInt &val);

    // operator *=, any of {CBigInt/int/string}
    CBigInt &operator*=(const CBigInt &val);

    // comparison operators, any combination {CBigInt/int/string} {<,<=,>,>=,==,!=} {CBigInt/int/string}
    friend bool operator<(const CBigInt &lhs, const CBigInt &rhs);

    friend bool operator<=(const CBigInt &lhs, const CBigInt &rhs);

    friend bool operator>(const CBigInt &lhs, const CBigInt &rhs);

    friend bool operator>=(const CBigInt &lhs, const CBigInt &rhs);

    friend bool operator==(const CBigInt &lhs, const CBigInt &rhs);

    friend bool operator!=(const CBigInt &lhs, const CBigInt &rhs);

    // output operator <<
    friend std::ostream &operator<<(std::ostream &out, const CBigInt &val);

    // input operator >>
    friend std::istream &operator>>(std::istream &is, CBigInt &val);

private:
    bool is_positive;
    std::vector<unsigned int> value;
};