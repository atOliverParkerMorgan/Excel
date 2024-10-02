#ifndef __PROGTEST__

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */

template<typename T_>
struct CArray {
    CArray() = default;

    explicit CArray(size_t size, const T_ &init) : m_Size(size), m_Data(new T_[size]) {
        if (size == 0) return;
        for (int i = 0; i < m_Size; ++i) {
            m_Data[i] = init;
        }
    };

    CArray(const CArray<T_> &other) {
        *this = other;
    }

    ~CArray() { delete[] m_Data; }

    CArray<T_> &operator=(const CArray<T_> &other) {
        if (this != &other) {
            T_ *tmp = new T_[m_Size = other.m_Size];
            delete[]m_Data;
            m_Data = tmp;
            for (int i = 0; i < m_Size; ++i) {
                m_Data[i] = other.m_Data[i];
            }
        }
        return *this;
    }

    T_ &operator[](size_t index) const {
        if (index >= m_Size) throw std::out_of_range("Index out of range.");
        return m_Data[index];
    }

    T_ *m_Data = nullptr;
    size_t m_Size = 0;
};


template<typename T_>
class CMatrix3 {

public:

    CMatrix3(unsigned n1, unsigned n2, unsigned n3) :
            m_Matrix(CArray<CArray<CArray<T_>>>(n1, CArray<CArray<T_>>(n2, CArray<T_>(n3, T_())))) {}

    ~CMatrix3() = default;

    CArray<CArray<T_>> &operator[](unsigned index) const {
        return m_Matrix[index];
    }

private:
    CArray<CArray<CArray<T_>>>
            m_Matrix;
};

#ifndef __PROGTEST__

int main(void) {
    CMatrix3<int> m(3, 4, 5);
    m[0][0][0] = 128;
    m[2][3][1] = 7;
    assert (m[0][0][0] == 128);
    assert (m[2][3][1] == 7);

    m[1][1][1] = m[0][0][0] + m[2][3][1];
    m[2][3][4] = 135;
    assert (m[1][1][1] == m[2][3][4]);

    CMatrix3<int> copy(m);
    try {
        copy[4][1][2];
        assert ("Missing exception for access i0 = 4, limit = 2" == nullptr);
    }
    catch (out_of_range &e) {}
    catch (...) { assert ("Incorrect exception thrown" == nullptr); }
    try {
        copy[1][4][2];
        assert ("Missing exception for access i1 = 4, limit = 3" == nullptr);
    }
    catch (out_of_range &e) {}
    catch (...) { assert ("Incorrect exception thrown" == nullptr); }
    try {
        copy[2][1][6];
        assert ("Missing exception for access i2 = 6, limit = 4" == nullptr);
    }
    catch (out_of_range &e) {}
    catch (...) { assert ("Incorrect exception thrown" == nullptr); }

    copy[2][3][1] = 42;
    assert (m[2][3][1] == 7);
    assert (copy[2][3][1] == 42);

    CMatrix3<int> n(2, 2, 2);
    n[0][1][0] = 56;
    n[1][0][1] = 42;
    m = n;
    n[0][1][0] = 0;
    assert (m[0][1][0] == 56);
    try {
        m[2][1][0];
        assert ("Missing exception for access i0 = 2, limit = 2" == nullptr);
    }
    catch (out_of_range &e) {}
    catch (...) { assert ("Incorrect exception thrown" == nullptr); }

    CMatrix3<string> s(2, 1, 1);
    s[0][0][0] = "Hello ";
    s[1][0][0] = "World!";
    assert ((s[0][0][0] + s[1][0][0]) == string("Hello World!"));

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */