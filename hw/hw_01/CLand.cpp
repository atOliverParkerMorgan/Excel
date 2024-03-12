#include "CLand.hpp"

CLand::CLand(std::string city, std::string addr, std::string region, unsigned int id) :
        city(std::move(city)),
        addr(std::move(addr)),
        region(std::move(region)),
        id(id) {}

void CLand::setOwner(const std::string &newOwner) {
    owner = newOwner;
}

bool CLand::operator==(const CLand &other) const {
    return city == other.city && addr == other.addr && region == other.region && id == other.id;
}
