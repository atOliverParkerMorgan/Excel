#ifndef __PROGTEST__

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>
#include <iostream>
#include <stdexcept>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;
#endif /* __PROGTEST__ */

template<typename T_>
class CSparseMatrix {
public:
    CSparseMatrix(int w, int h) : m_Width(w), m_Height(h) {
        if (w < 0 || h < 0) {
            throw out_of_range("Index error");
        }
    }

    ~CSparseMatrix() {
        for (int i = 0; i < m_Data.size(); ++i) {
            delete m_Data[i];
        }
    }

    CSparseMatrix(const CSparseMatrix &other) : m_Width(other.m_Width), m_Height(other.m_Height) {
        m_Width = other.m_Width;
        m_Height = other.m_Height;
        m_Cords = other.m_Cords;
        for (int i = 0; i < other.m_Data.size(); ++i) {
            m_Data.push_back(new T_(*other.m_Data[i]));
        }
    }

    CSparseMatrix &operator=(const CSparseMatrix &other) {
        if (this != &other) {
            m_Width = other.m_Width;
            m_Height = other.m_Height;
            m_Cords = other.m_Cords;
            for (int i = 0; i < m_Data.size(); ++i) {
                delete m_Data[i];
            }
            m_Data.erase(m_Data.begin(), m_Data.end());

            for (int i = 0; i < other.m_Data.size(); ++i) {
                m_Data.push_back(new T_(*other.m_Data[i]));
            }
        }
        return *this;
    }

    // destructor, operator =, copy constructor if needed
    T_ &operator()(int x, int y) {
        if (x >= m_Width || y >= m_Height || x < 0 || y < 0) {
            throw out_of_range("Index error");
        }

        pair<size_t, size_t> toFind = {x, y};
        auto it = lower_bound(m_Cords.begin(), m_Cords.end(), toFind);
        size_t index = distance(m_Cords.begin(), it);
        if (it == m_Cords.end() || *it != toFind) {
            m_Cords.insert(it, toFind);
            if (it == m_Cords.end()) {
                index = m_Cords.size();
            }
            m_Data.insert(m_Data.begin() + index, new T_());
        }


        return *m_Data[index];
    }

    bool contains(int x, int y) {
        pair<size_t, size_t> toFind = {x, y};
        auto it = lower_bound(m_Cords.begin(), m_Cords.end(), toFind);
        if (it == m_Cords.end() || *it != toFind) {
            return false;
        }
        return true;
    }

private:
    int m_Width;
    int m_Height;
    vector<T_ *> m_Data;
    vector<pair<size_t, size_t>> m_Cords;

};

#ifndef __PROGTEST__

int main(void) {
    CSparseMatrix<int> m1(300, 700);

    assert (!m1.contains(0, 0));
    m1(0, 0) = 123;
    m1(1, 1) = 321;
    m1(2, 2) = m1(1, 1) + m1(0, 0);
    assert (m1.contains(0, 0) && m1(0, 0) == 123);
    assert (m1.contains(1, 1) && m1(1, 1) == 321);
    assert (m1.contains(2, 2) && m1(2, 2) == 444);

    for (int i = 0; i < 300; ++i)
        for (int j = 0; j < 300; ++j)
            m1(i, j) = i * j;

    for (int i = 0; i < 300; ++i)
        for (int j = 0; j < 300; ++j)
            assert (m1.contains(i, j) && m1(i, j) == i * j);

    try {
        m1(300, 300) = 666;
        assert (false);
    }
    catch (out_of_range &o) {
        assert (o.what() == "Index error"s);
    }
    catch (...) {
        assert (false);
    }

    try {
        m1(299, 300) = 322;
    }
    catch (...) {
        assert (false);
    }

    CSparseMatrix m2 = m1;

    for (int i = 0; i < 300; ++i)
        for (int j = 0; j < 300; ++j)
            assert (m2.contains(i, j) && m2(i, j) == i * j);
    m1(0, 0) = 15;

    assert (m2(0, 0) == 0);
    assert (m1(0, 0) == 15);

    CSparseMatrix<int> m3(1000, 1000);
    assert (!m3.contains(666, 666));
    m3(666, 666) = 666;
    assert (m3.contains(666, 666) && m3(666, 666) == 666);
    m3 = m1;
    assert (!m3.contains(666, 666));
    assert (m3.contains(0, 0) && m3(0, 0) == 15);

    for (int i = 1; i < 300; ++i)
        for (int j = 1; j < 300; ++j)
            assert (m3.contains(i, j) && m3(i, j) == i * j);

    CSparseMatrix<string> m4(1000000, 1000000);
    assert (!m4.contains(0, 0));
    assert (!m4.contains(999999, 999999));
    m4(0, 0) = "P";
    m4(10, 40) = "r";
    m4(55, 50000) = "o";
    m4(400000, 11) = "g";
    m4(999999, 1221) = "t";
    m4(121, 169) = "e";
    m4(66, 6) = "s";
    m4(6, 66) = "t";
    assert (m4.contains(0, 0) && m4(0, 0) == "P"s);
    assert (m4.contains(10, 40) && m4(10, 40) == "r"s);
    assert (m4.contains(55, 50000) && m4(55, 50000) == "o"s);
    assert (m4.contains(400000, 11) && m4(400000, 11) == "g"s);
    assert (m4.contains(999999, 1221) && m4(999999, 1221) == "t"s);
    assert (m4.contains(121, 169) && m4(121, 169) == "e"s);
    assert (m4.contains(66, 6) && m4(66, 6) == "s"s);
    assert (m4.contains(6, 66) && m4(6, 66) == "t"s);
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */