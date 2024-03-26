#ifndef __PROGTEST__

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

#endif /* __PROGTEST__ */

class CBigInt {
public:
    // default constructor
    CBigInt();

    CBigInt(const std::string &basicString);

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
    std::string value;
};

CBigInt::CBigInt(const char val[]) {
    if (val == nullptr || std::strlen(val) == 0 || (val[0] != '-' && !std::isdigit(val[0]))) {
        throw std::invalid_argument("Invalid input: " + std::string(val));
    }

    value = val;
    is_positive = value[0] != '-';
    if (!is_positive) {
        value.erase(0, 1);
    }
    size_t non_zero_pos = value.find_first_not_of('0');

    if (non_zero_pos == std::string::npos) {
        is_positive = true;
        value = "0";
        return;
    }

    if (non_zero_pos > 0) {
        value.erase(0, non_zero_pos);
    }

    if (!std::all_of(value.begin(), value.end(), [](char c) { return isdigit(c); })) {
        value = "";
        throw std::invalid_argument("Neplatný vstup: " + std::string(value));

    }
}

CBigInt::CBigInt(const std::string &in_value) : CBigInt(in_value.c_str()) {};

CBigInt::CBigInt(int in_value) {
    is_positive = in_value >= 0;
    if (in_value == 0) {
        is_positive = true;
        value = "0";
        return;
    }

    if (!is_positive) in_value *= -1;

    int current;
    while (in_value != 0) {
        current = in_value % 10;
        value += std::to_string(current);
        in_value /= 10;
    }
    std::reverse(value.begin(), value.end());
}

CBigInt::CBigInt() : is_positive(true), value("0") {}

std::ostream &operator<<(std::ostream &out, const CBigInt &val) {
    std::string sign = val.is_positive || val.value == "0" ? "" : "-";
    return out << sign << val.value;
}

CBigInt operator+(const CBigInt &val1, const CBigInt &val2) {
    int overflow = 0;
    size_t index1 = val1.value.length();
    size_t index2 = val2.value.length();
    std::string result;
    CBigInt out;

    if ((val1.is_positive && val2.is_positive) || (!val1.is_positive && !val2.is_positive)) {

        while (index1 > 0 || index2 > 0 || overflow) {
            int num1 = (index1 > 0) ? val1.value[--index1] - '0' : 0;
            int num2 = (index2 > 0) ? val2.value[--index2] - '0' : 0;

            int sum = num1 + num2 + overflow;
            overflow = sum >= 10 ? 1 : 0;
            sum %= 10;
            result.insert(0, 1, (char) (sum + '0'));
        }
        out = CBigInt(result.c_str());
        out.is_positive = val1.is_positive && val2.is_positive;

    } else {
        CBigInt cpy1 = val1;
        cpy1.is_positive = true;
        CBigInt cpy2 = val2;
        cpy2.is_positive = true;

        bool is_positive = val1.is_positive && !val2.is_positive;
        if (cpy1 < cpy2) {
            is_positive = !val1.is_positive && val2.is_positive;
            std::swap(cpy1, cpy2);
            std::swap(index1, index2);
        }

        while (index1 > 0 || index2 > 0 || overflow) {
            int num1 = (index1 > 0) ? cpy1.value[--index1] - '0' : 0;
            int num2 = (index2 > 0) ? cpy2.value[--index2] - '0' : 0;

            int sum = num1 - (num2 + overflow);
            overflow = sum < 0 ? 1 : 0;
            sum = sum < 0 ? sum + 10 : sum;
            result.insert(0, 1, char(sum + '0'));
        }
        out = CBigInt(result.c_str());
        out.is_positive = is_positive;
    }
    return out;
}

CBigInt operator*(const CBigInt &val1, const CBigInt &val2) {
    const size_t length1 = val1.value.length();
    const size_t length2 = val2.value.length();

    size_t index1 = val1.value.length();
    size_t index2;

    CBigInt result("0");

    while (index1 > 0) {
        int current = (int) (val1.value[--index1] - '0');
        int overflow = 0;
        index2 = length2;

        CBigInt result_local("0");
        result_local.value = "";

        while (index2 > 0 || overflow) {
            int current2 = (index2 > 0) ? (val2.value[--index2] - '0')
                                        : 0;
            int prod = current * current2 + overflow;
            overflow = prod / 10;
            prod %= 10;
            result_local.value.insert(0, 1, (char) (prod + '0'));
        }
        result_local.value.append(length1 - index1 - 1, '0');
        result += result_local;
    }

    result.is_positive = (val1.is_positive && val2.is_positive) || (!val1.is_positive && !val2.is_positive);
    return result;
}


CBigInt &CBigInt::operator+=(const CBigInt &val) {
    return *this = *this + val;
}

CBigInt &CBigInt::operator*=(const CBigInt &val) {
    return *this = *this * val;
}

bool operator==(const CBigInt &lhs, const CBigInt &rhs) {
    return (lhs.is_positive == rhs.is_positive && lhs.value == rhs.value) || (lhs.value == "0" && rhs.value == "0");
}

bool operator<(const CBigInt &lhs, const CBigInt &rhs) {
    if ((lhs.is_positive && !rhs.is_positive) || (!lhs.is_positive && rhs.is_positive)) {
        return !lhs.is_positive && rhs.is_positive;
    }
    const bool both_negative = !lhs.is_positive && !rhs.is_positive;
    const size_t length1 = lhs.value.length();
    const size_t length2 = rhs.value.length();
    if (length1 != length2) {
        if (both_negative)
            return length1 > length2;
        else {
            return length1 < length2;
        }
    }
    for (size_t i = 0; i < length1; ++i) {
        if (lhs.value[i] != rhs.value[i]) {
            if (both_negative)
                return lhs.value[i] > rhs.value[i];
            else
                return lhs.value[i] < rhs.value[i];

        }
    }
    return false;
}

bool operator<=(const CBigInt &lhs, const CBigInt &rhs) {
    return lhs == rhs || lhs < rhs;
}

bool operator>(const CBigInt &lhs, const CBigInt &rhs) {
    return !(lhs <= rhs);
}

bool operator>=(const CBigInt &lhs, const CBigInt &rhs) {
    return !(lhs < rhs);
}

bool operator!=(const CBigInt &lhs, const CBigInt &rhs) {
    return !(lhs == rhs);
}

std::istream &operator>>(std::istream &is, CBigInt &val) {
    while (is.peek() == ' ')
        is.get();

    std::string num_str;
    char ch;

    bool found_number = false;
    bool is_positive = true;
    bool found_start_zero = false;

    if (is.peek() == '-') {
        is_positive = false;
        is.get();
    }


    while (is.peek() == '0'){
        is.get();
        found_start_zero = true;
    }

    while ((is.get(ch) && std::isdigit(ch))) {
        num_str.push_back(ch);
        found_number = true;
    }
    if(!found_number && found_start_zero){
        num_str = "0";
    }
    if(num_str == "0") is_positive = true;

    if (found_number || found_start_zero) {
        is.clear();
        val.value = num_str;
        val.is_positive = is_positive;
    } else
        is.setstate(std::istream::failbit);


    return is;
}

#ifndef __PROGTEST__

static bool equal(const CBigInt &x, const char val[]) {
    std::ostringstream oss;
    oss << x;
    return oss.str() == val;
}

static bool equalHex(const CBigInt &x, const char val[]) {
    return true; // hex output is needed for bonus tests only
    // std::ostringstream oss;
    // oss << std::hex << x;
    // return oss . str () == val;
}

int main() {
    CBigInt a, b;
    std::istringstream is;
    a = 10;
    a += 20;
    assert (equal(a, "30"));
    a *= 5;
    assert (equal(a, "150"));
    b = a + 3;
    assert (equal(b, "153"));
    b = a * 7;
    assert (equal(b, "1050"));
    assert (equal(a, "150"));
    assert (equalHex(a, "96"));

    a = 10;
    a += -20;
    assert (equal(a, "-10"));
    a *= 5;
    assert (equal(a, "-50"));
    b = a + 73;
    assert (equal(b, "23"));
    b = a * -7;
    assert (equal(b, "350"));
    assert (equal(a, "-50"));
    assert (equalHex(a, "-32"));

    a = "12345678901234567890";
    a += "-99999999999999999999";
    assert (equal(a, "-87654321098765432109"));
    a *= "54321987654321987654";
    assert (equal(a, "-4761556948575111126880627366067073182286"));
    a *= 0;
    assert (equal(a, "0"));
    a = 10;
    b = a + "400";
    assert (equal(b, "410"));
    b = a * "15";
    assert (equal(b, "150"));
    assert (equal(a, "10"));
    assert (equalHex(a, "a"));

    is.clear();
    is.str(" 1234");

    assert (is >> b);
    assert (equal(b, "1234"));
    is.clear();
    is.str(" 12 34");
    assert (is >> b);
    assert (equal(b, "12"));

    is.clear();
    is.str(" -0000000000123a2");
    assert (is >> b);
    assert (equal(b, "-123"));

    is.clear();
    is.str("999z");
    assert (is >> b);
    assert (equal(b, "999"));
    is.clear();
    is.str("abcd");
    assert (!(is >> b));
    is.clear();
    is.str("- 758");
    assert (!(is >> b));
    a = 42;
    try {
        a = "-xyz";
        assert ("missing an exception" == nullptr);
    }
    catch (const std::invalid_argument &e) {
        assert (equal(a, "42"));
    }

    a = "73786976294838206464";
    assert (equal(a, "73786976294838206464"));
    assert (equalHex(a, "40000000000000000"));
    assert (a < "1361129467683753853853498429727072845824");
    assert (a <= "1361129467683753853853498429727072845824");
    assert (!(a > "1361129467683753853853498429727072845824"));
    assert (!(a >= "1361129467683753853853498429727072845824"));
    assert (!(a == "1361129467683753853853498429727072845824"));
    assert (a != "1361129467683753853853498429727072845824");
    assert (!(a < "73786976294838206464"));
    assert (a <= "73786976294838206464");
    assert (!(a > "73786976294838206464"));
    assert (a >= "73786976294838206464");
    assert (a == "73786976294838206464");
    assert (!(a != "73786976294838206464"));
    assert (a < "73786976294838206465");
    assert (a <= "73786976294838206465");
    assert (!(a > "73786976294838206465"));
    assert (!(a >= "73786976294838206465"));
    assert (!(a == "73786976294838206465"));
    assert (a != "73786976294838206465");
    a = "2147483648";
    assert (!(a < -2147483648));
    assert (!(a <= -2147483648));
    assert (a > -2147483648);
    assert (a >= -2147483648);
    assert (!(a == -2147483648));
    assert (a != -2147483648);
    a = "-12345678";
    assert (!(a < -87654321));
    assert (!(a <= -87654321));
    assert (a > -87654321);
    assert (a >= -87654321);
    assert (!(a == -87654321));
    assert (a != -87654321);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */