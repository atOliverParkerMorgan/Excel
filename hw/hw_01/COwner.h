
#include <string>
#include <vector>
#include "CLand.hpp"

class COwner {
public:
    explicit COwner(const std::string& name);
    std::string nameLower;
    size_t acquiredCount;
    std::vector<CLand *> owned;

    static std::string stringToLower(const std::string &in);
    std::vector<CLand *> getSortedCopyByAcquire() const;
};

