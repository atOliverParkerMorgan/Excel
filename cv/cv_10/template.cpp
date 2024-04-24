#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>

#define LEVEL 5

/* POPIS LEVELU
1: Kontruktor, vypis, size(), push_back() a pop_back() s jednim parametrem - 1b
2: Kopirovaci konstruktor, operator prirazeni
3: Indexovaci operator a vyjimky - 2b
4: Inicializace pomoci initializer list
5: Vkladani pomoci push_back s vice argumenty - 3b
6: Sorteni sort(bool asc)
7: Sorteni vlastni funkci - 4b
*/



struct MyInt {
    int m_Val;

    MyInt(int val = 0) : m_Val(val) {}

    friend bool operator<(const MyInt &lhs, const MyInt &rhs) {
        return lhs.m_Val < rhs.m_Val;
    }

    friend std::ostream &operator<<(std::ostream os, const MyInt &x) {
        return os << x.m_Val;
    }
};

// TODO: Vytvorit Array pomoci template
template<typename T>
class Array {
private:
    size_t m_size;
    size_t m_capacity;
    T *elements;
public:
    Array() : m_size(0), m_capacity(8), elements(new T[m_capacity]) {}

    Array(std::initializer_list<T> list) : m_size(list.size()), m_capacity(2 * list.size()),
                                           elements(new T[m_capacity]) {
        size_t i = 0;
        for (const auto &item: list) {
            elements[i++] = item;
        }
    }

    ~Array() {
        delete[] elements;
    }

    Array(const Array &array) : m_size(array.m_size), m_capacity(array.m_capacity), elements(new T[m_capacity]) {
        std::copy(array.elements, array.elements + array.m_size, elements);
    }

    Array &operator=(const Array &array) {
        if (this != &array) {
            delete[] elements;

            m_size = array.m_size;
            m_capacity = array.m_capacity;

            elements = new T[m_capacity];
            for (size_t i = 0; i < m_size; ++i) {
                elements[i] = array.elements[i];
            }
        }
        return *this;
    }

    T &operator[](size_t index) const {
        if (index >= m_size) {
            throw std::runtime_error("Index out of range");
        }
        return elements[index];
    }

    friend std::ostream &operator<<(std::ostream &ostream, const Array &array) {
        ostream << "{";
        for (int i = 0; i < array.m_size; ++i) {
            ostream << array.elements[i];
            ostream << ((array.m_size == i + 1) ? "" : ", ");
        }
        ostream << "}";
        return ostream;
    }


    size_t size() const {
        return m_size;
    }

    T &operator[](size_t index) {
        if (index >= m_size) {
            throw std::runtime_error("Index out of range");
        }
        return elements[index];
    }

    /*
     * template<typename ...Args>
     * Array &push_back(Args ... args){
     *  push_back(val);
     *  return push_back(...args);
     * }
     * */

    Array &push_back(const auto &... rest) {
        (push_back(rest), ...);
        return *this;
    }


    Array &push_back(const T &value) {
        if (m_size >= m_capacity) {
            m_capacity *= 2; // Double the capacity
            T *newElements = new T[m_capacity];
            for (size_t i = 0; i < m_size; ++i) {
                newElements[i] = elements[i];
            }
            delete[] elements;
            elements = newElements;
        }
        elements[m_size++] = value;

        return *this;

    }

    Array &pop_back() {
        if (m_size <= 0) {
            throw std::runtime_error("42");
        }
        if (m_size > 0) {
            --m_size;
        }
        return *this;
    }

    void sort(bool asc = true) {
        if (asc) {
            std::stable_sort(elements, elements + m_size);
        } else {
            std::stable_sort(elements, elements + m_size, [&](const T &lhs, const T &rhs) {
                return rhs < lhs;
            });
        }
    }

};

#if LEVEL >= 1

template<typename T>
void assertPrint(const Array<T> &arr, const std::string_view &sw) {
    std::stringstream ss;
    ss << arr;
    std::cout << arr << std::endl;
    assert(ss.str() == sw);
}

#endif

int main() {

#if LEVEL >= 1
    // Kontruktor, vypis, size, push, pop
    Array<int> arr0;
    assertPrint(arr0, "{}");
    assert(arr0.size() == 0);

    arr0.push_back(42);
    assert(arr0.size() == 1);
    assertPrint(arr0, "{42}");

    arr0.push_back(666).push_back(113);
    assert(arr0.size() == 3);
    assertPrint(arr0, "{42, 666, 113}");

    arr0.pop_back();
    assert(arr0.size() == 2);
    assertPrint(arr0, "{42, 666}");

    arr0.pop_back().pop_back().push_back(-1);
    assert(arr0.size() == 1);
    assertPrint(arr0, "{-1}");

    Array<unsigned> long_array;
    for (unsigned i = 0; i < 1000000; i++)
        long_array.push_back(i * 5);
    assert(long_array.size() == 1000000);
    long_array.pop_back();
    assert(long_array.size() == 999999);
#endif

#if LEVEL >= 2
    // kopirovaci konstruktor, operator prirazeni
    arr0.push_back(42).push_back(333);
    Array<int> arr2 = arr0;
    assert(arr2.size() == 3);
    arr2.pop_back().pop_back();
    assert(arr0.size() == 3 && arr2.size() == 1);

    Array<int> arr2a;
    assertPrint(arr2a, "{}");
    assert(arr2a.size() == 0);

    arr2a.push_back(112).push_back(155).push_back(150).push_back(158);
    arr2a = arr0;
    assert(arr0.size() == 3 && arr2a.size() == 3);

    arr2a.push_back(9);
    assert(arr0.size() == 3 && arr2a.size() == 4);

    arr2a = arr2a;
    assert(arr2a.size() == 4);
    arr2a = arr2 = arr2a;
    assert(arr2.size() == 4 && arr2a.size() == 4);

#endif

#if LEVEL >= 3
    // indexovaci operator a vyjimky
    assert(arr0[0] == -1);
    assert(arr0[1] == 42);
    assert(arr0[2] == 333);
    try {
        std::cout << arr0[3] << std::endl;
        assert("missing exception" == nullptr);
    }
    catch (const std::runtime_error &err) {
    }

    const Array<int> constarray = arr0;
    assert(constarray[1] == 42);
    // assert(constarray[1] = 0); // NELZE

    arr0[0] = 100;
    assert(arr0[0] == 100);
#endif

#if LEVEL >= 4
    // initializer list
    Array<int> arr3{1, 2, 3};
    assert(arr3.size() == 3);
    arr3.pop_back()
            .pop_back()
            .pop_back();
    try {
        arr3.pop_back();
        assert("missing exception" == nullptr);
    }
    catch (const std::runtime_error &err) {
    }
    arr3.push_back(2).push_back(6);
    assert(arr3.size() == 2);

    arr3 = {2, 3, 5, 7, 11, 13, 17};
    assert(arr3.size() == 7);
#endif

#if LEVEL >= 5//
    //    Array& push_back(const T& value, const T &... rest) {
    //        push_back(value);
    //        push_back(std::forward<Args>(rest)...);
    //        return *this;
    //    }

    // push s vice argumenty
    Array<int> arr1;
    assertPrint(arr1, "{}");
    assert(arr1.size() == 0);
    arr1.push_back(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24);
    assert(arr1[3] == 8);
    assert(arr1.size() == 12);
    assertPrint(arr1, "{2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24}");
    arr1.pop_back().push_back(25);
    assert(arr1.size() == 12);
    assertPrint(arr1, "{2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 25}");
    try {
        std::cout << arr1[12] << std::endl;
        assert("missing exception" == nullptr);
    }
    catch (const std::runtime_error &err) {
    }
#endif

#if LEVEL >= 6
    // sorteni
    Array<MyInt> arrInt{4, 1, 3, 5, 2};
    arrInt[3] = 7;
    arrInt.sort();
    assertPrint(arrInt, "{1, 2, 3, 4, 7}");
    arrInt = {6, 10, 7, 9, 8};
    arrInt.sort(false);
    assertPrint(arrInt, "{10, 9, 8, 7, 6}");
    arrInt.sort(true);
    assertPrint(arrInt, "{6, 7, 8, 9, 10}");

    Array<std::string> arrStr{"abc", "ghi", "def"};
    arrStr.sort(false);
    assertPrint(arrStr, "{ghi, def, abc}");

#endif

#if LEVEL >= 7
    // sorteni vlastni funkci
    Array arrMod{3, 1, 3, 7, 2, 0, 9};
    arrMod.push_back(5, 6, 5, 0, 8, 1);
    arrMod.sort([&](int lhs, int rhs) { return lhs % 3 < rhs % 3; });
    assertPrint(arrMod, "{3, 3, 0, 9, 6, 0, 1, 7, 1, 2, 5, 5, 8}");

    Array<int> arrayExtra;
    for (int i = 1; i < 21; i++) {
        arrayExtra.push_back(i);
    }
    arrayExtra.sort([&](int lhs, int rhs) { return lhs % 3 < rhs % 3; });
    assertPrint(arrayExtra, "{3, 6, 9, 12, 15, 18, 1, 4, 7, 10, 13, 16, 19, 2, 5, 8, 11, 14, 17, 20}");

#endif
}
