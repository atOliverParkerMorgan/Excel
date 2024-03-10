#include "Land.hpp"

Land::Land(std::string city, std::string addr, std::string region, unsigned int id) :
        city(std::move(city)),
        addr(std::move(addr)),
        region(std::move(region)),
        id(id){}

void Land::setOwner(const std::string &newOwner) {
    owner = newOwner;
}
