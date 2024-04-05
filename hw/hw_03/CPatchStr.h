#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>

class CPatchStr {
public:
    struct Node {

        std::shared_ptr<char[]> patch;
        size_t offset;
        size_t len;

        Node();

        Node(const Node &node);

        Node &operator=(const Node &rhs);
    };

    struct PatchList {
        std::unique_ptr<Node[]> nodes;
        size_t capacity;
        size_t size;
        size_t strLen;

        PatchList();
        PatchList(const PatchList &patchList);

    };


    std::shared_ptr<PatchList> patchList;

    CPatchStr();

    CPatchStr(const char *str);

    CPatchStr(const CPatchStr &other);

    ~CPatchStr();

    CPatchStr &operator=(const CPatchStr &rhs);

    // copy constructor
    // destructor
    // operator =
    CPatchStr subStr(size_t from,
                     size_t len) const;

    CPatchStr &append(const CPatchStr &src);

    CPatchStr &insert(size_t pos,
                      const CPatchStr &src);

    CPatchStr &remove(size_t from,
                      size_t len);

    char *toStr() const;

private:
    void push_back(const char *str, size_t length);
};