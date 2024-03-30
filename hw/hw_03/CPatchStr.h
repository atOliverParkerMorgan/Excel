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
    void print();


private:


    struct CNode {
        CNode *next = nullptr;
        std::shared_ptr<char> sharedPtr;
        size_t ofs = 0;
        size_t len;

        CNode();

        CNode(const std::shared_ptr<char> &ptr);
        CNode(const char*  string);

        ~CNode();
    };

    CNode *getCStrOnPos(size_t pos);

    CPatchStr::CNode *head;
    CPatchStr::CNode *tail;
    size_t len = 0;
};
