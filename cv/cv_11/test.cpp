#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cassert>
#include <limits>
#include <algorithm>

#define UKOL0 // Definice typu vektoru intu
#define UKOL1   // Hashovani
#define UKOL2   // Filtrovani vektoru
#define UKOL3   // Razeni podle zadani
#define UKOL4   // Komprese vektoru
#define UKOL5   // Range fce pro vektor
#define UKOL6   // Fce SortAlpha

#define PRINTRESULTS 1

#ifdef UKOL0
typedef std::vector<int> int_vect;
#endif

// stream operator pro vektor intu
std::ostream &operator<<(std::ostream &os, const int_vect &v) {
    for (int x: v)
        os << x << " ";
    return os;
}

// TODO: hash functor
struct HashString {
    size_t operator()(const std::string &s) const {
        size_t hash = 0;
        for (int i = 0; i < s.length(); ++i) {
            hash += static_cast<int>(s[i]);
        }
        return hash;
    }
};

// TODO: funkce pro filtrovani duplikatu ve vektoru, ktera vraci take vektor
int_vect filter_duplicates(const int_vect &vector) {
    std::set<int> tmp;
    int_vect newVector;
    for (int i: vector) {
        tmp.insert(i);
    }
    for (int element: tmp) {
        newVector.push_back(element);
    }
    return newVector;
}

// TODO: funkce pro nase specialni razeni
int_vect sort_data(const int_vect &v) {
    std::map<int, size_t> counter;
    std::vector<std::pair<int, size_t>> tmp;
    for (int x: v) {
        counter[x]++;
    }

    tmp.reserve(counter.size());
    for (auto a: counter) {
        tmp.emplace_back(a);
    }
    std::sort(tmp.begin(), tmp.end(), [](const std::pair<int, size_t> &a, const std::pair<int, size_t> &b) {
        return a.second > b.second || a.second == b.second && a.first < b.first;
    });

    int_vect newVector;
    for (auto el: tmp) {
        for (int x = 0; x < el.second; x++) {
            newVector.push_back(el.first);
        }
    }
    std::cout << std::endl;

    return newVector;
}

// TODO: funktor pro kompresi rozsahu vektoru
class CompressVector { ;

public:
    int_vect operator()(const int_vect &v) const {
        int_vect newVec = v;
        int_vect tmp = filter_duplicates(v);
        for (int i = 0; i < newVec.size(); ++i) {
            for (int j = 0; j < tmp.size(); ++j) {
                if (tmp[j] == v[i]) {
                    newVec[i] = j + m_Size;
                }
            }
        }

        return newVec;
    }

    CompressVector(int n = 1) : m_Size(n) {}

private:
    int m_Size = 1;

};

// TODO: funkce SortAlpha()
std::string SortAlpha(std::string string, std::string order) {
    std::sort(string.begin(), string.end(), [&order](char a, char b) { return order.find(a) < order.find(b); });
    return string;
}

int main() {
#ifdef UKOL0
    int_vect data = {1, 4, 1, 7, 4, 5, 7, 7};

    if (PRINTRESULTS)
        std::cout << "Input data:       " << data << std::endl;
    std::cout << "UKOL0: OK" << std::endl;
#endif

#ifdef UKOL1 // hash functor
    HashString hash;

    std::string s1 = "AA";
    std::string s2 = "    ";

    assert(hash(s1) == 130);
    assert(hash(s2) == 128);

    if (PRINTRESULTS)
        std::cout << "String hash: " << hash(s1) << std::endl;
    if (PRINTRESULTS)
        std::cout << "String hash: " << hash(s2) << std::endl;

    std::cout << "UKOL1: OK" << std::endl;
#endif

#ifdef UKOL2 // filtrovani pole pomoci funkce
    int_vect filtered = filter_duplicates(data);
    assert(filtered == int_vect({1, 4, 5, 7}));

    int_vect filtered2 = filter_duplicates({3, 2, 1, 1, 2, 3, 3, 2, 3});
    assert(filtered2 == int_vect({1, 2, 3}));

    if (PRINTRESULTS)
        std::cout << "Filtered data:    " << filtered << std::endl;
    if (PRINTRESULTS)
        std::cout << "Filtered data:    " << filtered2 << std::endl;

    std::cout << "UKOL2: OK" << std::endl;
#endif

#ifdef UKOL3 // serazeni dat podle zadani pomoci funkce
    int_vect sorted = sort_data(data);
    assert(sorted == int_vect({7, 7, 7, 1, 1, 4, 4, 5}));

    if (PRINTRESULTS)
        std::cout << "Sorted data:      " << sorted << std::endl;

    std::cout << "UKOL3: OK" << std::endl;
#endif

#ifdef UKOL4 // komprese vektoru podle zadani s volitelnym zacatkem
    CompressVector c1;
    CompressVector c2(10);

    int_vect compressed1 = c1(data);
    int_vect compressed2 = c2(data);

    assert(compressed1 == int_vect({1, 2, 1, 4, 2, 3, 4, 4}));
    assert(compressed2 == int_vect({10, 11, 10, 13, 11, 12, 13, 13}));

    if (PRINTRESULTS)
        std::cout << "Compressed data1: " << compressed1 << std::endl;
    if (PRINTRESULTS)
        std::cout << "Compressed data2: " << compressed2 << std::endl;

    std::cout << "UKOL4: OK" << std::endl;
#endif

#ifdef UKOL5
    int_vect v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int_vect v2 = {0, 0};
    int_vect v3 = {3, 1, 4, 1, 5, 2, 6};

    if (PRINTRESULTS)
        std::cout << "v1: " << v1 << std::endl;
    if (PRINTRESULTS)
        std::cout << "v2: " << v2 << std::endl;
    if (PRINTRESULTS)
        std::cout << "v3: " << v3 << std::endl;

    // nahradit data ve v2 druhou polovinu prvku z v1
    std::cout << std::endl;

    int val = v1.size() / 2;
    for (int i = (int) (val); i < v1.size(); ++i) {
        if (v2.size() > i - val) {
            v2[i - (val)] = v1[i];
        } else {
            v2.push_back(v1[i]);
        }
    }

    if (PRINTRESULTS)
        std::cout << "v2: " << v2 << std::endl;

    assert(v2 == int_vect({6, 7, 8, 9, 10}));

    // vlozit druhou polovinu prvku z v1 za data ve v3
    for (int i = v1.size() / 2; i < v1.size(); ++i) {
        v3.push_back(v1[i]);
    }

    if (PRINTRESULTS)
        std::cout << "v3: " << v3 << std::endl;

    assert(v3 == int_vect({3, 1, 4, 1, 5, 2, 6, 6, 7, 8, 9, 10}));

#endif

#ifdef UKOL6

    assert(SortAlpha("abceacebcaaecb", {'e', 'c', 'b', 'd', 'a'}) == "eeeccccbbbaaaa");
    assert(SortAlpha("abceacebcaaecb", {'a', 'd', 'c', 'b', 'e'}) == "aaaaccccbbbeee");
    assert(SortAlpha("abceacebcaaecb", {'c', 'e', 'a', 'd', 'b'}) == "cccceeeaaaabbb");

#endif

    return 0;
}
