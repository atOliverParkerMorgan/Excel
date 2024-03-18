#include "CBigInt.h"

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

    bool is_first = true;
    bool found_number = false;
    bool is_positive = true;


    while ((is.get(ch) && std::isdigit(ch)) || (is_first && ch == '-')) {
        if (is_first && ch == '-') {
            is_positive = false;
        } else {
            num_str.push_back(ch);
            found_number = true;
        }
        is_first = false;

    }
    if (is) {
        is.unget();
    }
    if (found_number) {
        is.clear();
        val.value = num_str;
        val.is_positive = is_positive;
    } else
        is.setstate(std::istream::failbit);


    return is;
}

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
