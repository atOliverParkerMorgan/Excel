#include "Land.hpp"

Land::Land(std::string city, std::string addr, std::string region, unsigned int id, int acquire) :
        city(std::move(city)),
        addr(std::move(addr)),
        region(std::move(region)),
        id(id),
        acquireOrder(acquire) {}

void Land::setOwner(const std::string &newOwner) {
    std::string result;
    for (char c: newOwner) {
        result += static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    ownerLower = result;
    owner = newOwner;
}
