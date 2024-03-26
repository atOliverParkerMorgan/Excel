#include "CBigInt.h"

CBigInt::CBigInt() {

}

CBigInt::CBigInt(int value) {

}

CBigInt::CBigInt(const char *in_value) {

}

CBigInt operator+(const CBigInt &val1, const CBigInt &val2) {
    return CBigInt();
}

CBigInt operator*(const CBigInt &val1, const CBigInt &val2) {
    return CBigInt();
}

CBigInt &CBigInt::operator+=(const CBigInt &val) {
    return <#initializer#>;
}

CBigInt &CBigInt::operator*=(const CBigInt &val) {
    return <#initializer#>;
}

bool operator<(const CBigInt &lhs, const CBigInt &rhs) {
    return false;
}

std::istream &operator>>(std::istream &is, CBigInt &val) {
    return <#initializer#>;
}

std::ostream &operator<<(std::ostream &out, const CBigInt &val) {
    return <#initializer#>;
}

bool operator!=(const CBigInt &lhs, const CBigInt &rhs) {
    return false;
}

bool operator==(const CBigInt &lhs, const CBigInt &rhs) {
    return false;
}

bool operator>=(const CBigInt &lhs, const CBigInt &rhs) {
    return false;
}

bool operator>(const CBigInt &lhs, const CBigInt &rhs) {
    return false;
}

bool operator<=(const CBigInt &lhs, const CBigInt &rhs) {
    return false;
}
