#ifndef __PROGTEST__

#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <algorithm>
#include <memory>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */

template<typename T_>
class CDiagonalMatrix {

public:


    CDiagonalMatrix(int n, int k) : m_N(n), m_K(k) {
        if (k > n || k <= 0) {
            throw out_of_range("invalid matrix size");
        }

        const size_t numberOfDiagonals = 2 * (m_K - 1) + 1;
        m_MainDiagonalIndex = numberOfDiagonals / 2;
        int length = m_N - m_K + 1;

        bool beforeMainDiagonal = true;

        for (size_t index = 0; index < numberOfDiagonals; ++index) {

            vector<T_> e;
            e.reserve(length);
            for (int i = 0; i < length; ++i) {
                e.emplace_back(T_());
            }
            m_Data.push_back(e);

            if (index == m_MainDiagonalIndex) {
                beforeMainDiagonal = false;
            }
            if (beforeMainDiagonal) length++;
            else length--;
        }
    }

    T_ &operator()(int x, int y) {
        if (x + m_K - 1 < y || y + m_K - 1 < x || x >= m_N || y >= m_N) {

            throw out_of_range("invalid index ( " + to_string(x) + ", " + to_string(y) + " )");
        }

        int diagonalIndex = x - y;
        if (diagonalIndex == 0) diagonalIndex = m_MainDiagonalIndex;
        else diagonalIndex = m_MainDiagonalIndex - diagonalIndex;

        return m_Data[diagonalIndex][min(x, y)];
    }

    bool exists(int x, int y) const {
        if (x + m_K - 1 < y || y + m_K - 1 < x || x >= m_N || y >= m_N) {
            return false;
        }
        return true;
    }

    void reshape(int k) {
        if (k > m_N || k <= 0) {
            throw out_of_range("invalid matrix size");
        }

        const int diff = k - m_K;
        if (diff < 0) {
            for (int i = 0; i < abs(diff); ++i) {
                m_Data.erase(m_Data.begin());
                if (!m_Data.empty()) m_Data.erase(m_Data.end());
            }
        } else if (diff > 0) {
            for (int i = 0; i < diff; ++i) {
                vector<T_> e;
                int length = m_N;

                if (!m_Data.empty()) {
                    length = m_Data[0].size() - 1;
                }

                e.reserve(length);
                for (int _ = 0; _ < length; ++_) {
                    e.emplace_back(T_());
                }
                m_Data.insert(m_Data.begin(), e);
                m_Data.insert(m_Data.end(), e);
            }
        }

        m_K = k;
        m_MainDiagonalIndex = (2 * (m_K - 1) + 1) / 2;
    }

private:
    int m_N;
    int m_K;
    size_t m_MainDiagonalIndex;
    vector<vector<T_>> m_Data;
};

#ifndef __PROGTEST__

class CTestCnt {
public:
    CTestCnt(int val = 0)
            : m_Val(val) {
        c_Inst++;
    }

    CTestCnt(const CTestCnt &src) noexcept
            : m_Val(src.m_Val) {
        c_Inst++;
    }

    CTestCnt(CTestCnt &&src) noexcept
            : m_Val(src.m_Val) {
        c_Inst++;
    }

    ~CTestCnt(void) noexcept {
        c_Inst--;
    }

    CTestCnt &operator=(CTestCnt src) noexcept {
        swap(src.m_Val, m_Val);
        return *this;
    }

    inline static size_t c_Inst = 0;
private:
    int m_Val;
};

int main(void) {
    CDiagonalMatrix<int> m1(5, 4);

    for (int i = 0; i < 5; i++)
        m1(i, i) = 10 + i;
    for (int i = 0; i < 4; i++) {
        m1(i, i + 1) = 20 + i;
        m1(i + 1, i) = 30 + i;
    }
    for (int i = 0; i < 3; i++) {
        m1(i, i + 2) = 40 + i;
        m1(i + 2, i) = 50 + i;
    }
    m1(0, 3) = 60;
    m1(1, 4) = 61;
    m1(3, 0) = 70;
    m1(4, 1) = 71;

    CDiagonalMatrix<int> m2(m1);

    for (int i = 0; i < 5; i++)
        assert (m2(i, i) == 10 + i);
    for (int i = 0; i < 4; i++) {
        assert (m2(i, i + 1) == 20 + i);
        assert (m2(i + 1, i) == 30 + i);
    }
    for (int i = 0; i < 3; i++) {
        assert (m2(i, i + 2) == 40 + i);
        assert (m2(i + 2, i) == 50 + i);
    }
    assert (m2(0, 3) == 60);
    assert (m2(1, 4) == 61);
    assert (m2(3, 0) == 70);
    assert (m2(4, 1) == 71);


    m2(0, 0) = 100;
    assert (m1(0, 0) == 10);
    assert (m2(0, 0) == 100);


    assert (m1.exists(4, 1) && m1(4, 1) == 71);
    m1.reshape(2);
    assert (m1.exists(0, 1) && m1(0, 1) == 20);
    assert (!m1.exists(0, 2));
    try {
        m1(0, 0) = m1(0, 2);
        assert ("Missing an exception" == nullptr);
    }
    catch (const std::out_of_range &e) {
        cout << e.what() << endl;
        assert (e.what() == "invalid index ( 0, 2 )"s);
    }


    m1.reshape(4);
    assert (m1.exists(0, 1) && m1(0, 1) == 20);
    assert (m1.exists(0, 2) && m1(0, 2) == 0);

    try {
        m1.reshape(6);
        assert ("Missing an exception" == nullptr);
    }
    catch (const std::out_of_range &e) {
        assert (e.what() == "invalid matrix size"s);
    }

    try {
        CDiagonalMatrix<int> m3(5, 6);
        assert ("Missing an exception" == nullptr);
    }
    catch (const std::out_of_range &e) {
        assert (e.what() == "invalid matrix size"s);
    }


    CDiagonalMatrix<CTestCnt> m10(12, 1);
    assert (CTestCnt::c_Inst == 12);
    m10.reshape(4);
    assert (CTestCnt::c_Inst == 72);
    m10.reshape(2);
    assert (CTestCnt::c_Inst == 34);
    m10.reshape(11);
    assert (CTestCnt::c_Inst == 142);
    m10.reshape(12);
    assert (CTestCnt::c_Inst == 144);
    m10.reshape(8);
    assert (CTestCnt::c_Inst == 124);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */