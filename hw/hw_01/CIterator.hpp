#include "iostream"
#include "Land.hpp"
#include <vector>
#include <type_traits>

class CIterator {
private:
    std::vector<Land> lands;
    size_t currentIndex;
public:
    CIterator(std::vector<Land> in_land);

    bool atEnd() const;

    void next();

    std::string city() const;

    std::string addr() const;

    std::string region() const;

    unsigned id() const;

    std::string owner() const;
};
