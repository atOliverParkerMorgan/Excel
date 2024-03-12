#include "COwner.h"
#include <utility>

COwner::COwner(const std::string& name) : nameLower(std::move(stringToLower(name))), acquiredCount(0) {}

std::vector<CLand *> COwner::getSortedCopyByAcquire() const {
    std::vector<CLand *> ownedLandVector = owned;

    std::sort(ownedLandVector.begin(), ownedLandVector.end(), [](const CLand *l1, const CLand *l2) {
        return l1->acquire < l2->acquire;
    });

    return ownedLandVector;
};

std::string COwner::stringToLower(const std::string &in) {
    std::string out;
    for (char c: in) {
        out += static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return out;
}
