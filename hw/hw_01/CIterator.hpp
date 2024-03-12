#include "iostream"
#include "COwner.h"
#include <vector>
#include <type_traits>

class CIterator {
private:
    std::vector<CLand*> lands;
    size_t currentIndex;
public:
    CIterator(std::vector<CLand *> in_land);

    bool atEnd() const;

    void next();

    std::string city() const;

    std::string addr() const;

    std::string region() const;

    unsigned id() const;

    std::string owner() const;
};
