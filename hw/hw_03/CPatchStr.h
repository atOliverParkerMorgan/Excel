#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>

// subStr(2, 5) = stfoo
// testfoobar
// tstfooestfoobar
class CPatchStr {
    size_t size;
    char *data;

public:


    CPatchStr();

    CPatchStr(const char *str);

    // copy constructor
    CPatchStr(const CPatchStr &other);

    // destructor
    ~CPatchStr();

    // operator =
    CPatchStr &operator=(const CPatchStr &rhs);

    CPatchStr subStr(size_t from,
                     size_t len) const;

    CPatchStr &append(const CPatchStr &src);

    CPatchStr &insert(size_t pos,
                      const CPatchStr &src);

    CPatchStr &remove(size_t from,
                      size_t len);

    char *toStr() const;

    void push(char * str);

};
