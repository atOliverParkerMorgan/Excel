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


    struct SharedPtr {
        size_t ref_count;
        char *data;

        void incRef();

        void decRef();

        SharedPtr();

        SharedPtr(const char *str);

        ~SharedPtr();
    };

    struct Str {
        size_t ofs;
        size_t len;
        SharedPtr *sharedPtr;

        Str();

        Str(const char *str);

        Str(const Str &other);

        ~Str();

    };

    size_t total_len = 0;
    size_t size;
    size_t cap;
    Str **data;

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
                      size_t length);

    char *toStr() const;

    void push(Str *str);

};
