#include "CLandRegister.hpp"

#include <utility>

bool CLandRegister::add(const std::string &city, const std::string &addr, const std::string &region, unsigned int id) {
    if (isDuplicate(city, addr) || isDuplicate(region, id)) {
        return false;
    }
    const Property property = Property(city, addr, region, id);
    auto insertPoint = std::upper_bound(propertyByRegionId.begin(), propertyByRegionId.end(), property,
                                        cmpByRegion);
    propertyByRegionId.insert(insertPoint, property);
    return true;
}

bool CLandRegister::del(const std::string &city, const std::string &addr) {
    return false;
}

bool CLandRegister::del(const std::string &region, unsigned int id) {
    return false;
}

bool CLandRegister::getOwner(const std::string &city, const std::string &addr, std::string &owner) const {
    return false;
}

bool CLandRegister::getOwner(const std::string &region, unsigned int id, std::string &owner) const {
    return false;
}

bool CLandRegister::newOwner(const std::string &city, const std::string &addr, const std::string &owner) {
    return false;
}

bool CLandRegister::newOwner(const std::string &region, unsigned int id, const std::string &owner) {
    return false;
}

size_t CLandRegister::count(const std::string &owner) const {
    return 0;
}

CIterator CLandRegister::listByAddr() const {
    return CIterator();
}

CIterator CLandRegister::listByOwner(const std::string &owner) const {
    return CIterator();
}


bool CLandRegister::isDuplicate(const std::string &region, unsigned int id) {
    const Property tmp = Property("", "", region, id);
    auto it = std::equal_range(propertyByRegionId.begin(), propertyByRegionId.end(), tmp, CLandRegister::cmpByRegion);
    return it.first != it.second;
}

bool CLandRegister::isDuplicate(const std::string &city, const std::string &addr) {
    const Property tmp = Property(city, addr, "", 0);
    auto it = std::equal_range(propertyByRegionId.begin(), propertyByRegionId.end(), tmp, CLandRegister::cmpByAddr);
    return it.first != it.second;
}

bool CLandRegister::cmpByAddr(const CLandRegister::Property &p1, const CLandRegister::Property &p2) {
    return (p1.addr > p2.addr) || (p1.addr == p2.addr && p1.city > p2.city);
};


bool CLandRegister::cmpByRegion(const CLandRegister::Property &p1, const CLandRegister::Property &p2) {
    return (p1.region > p2.region) || (p1.region == p2.region && p1.id > p2.id);
}

void CLandRegister::print(bool byCity) {
    if (byCity) {
        for (const Property &p: propertyByCityAddr) {
            std::cout << "[ ( City: " << p.city << ", Addr: " << p.addr << ") , Region: " << p.region << ", ID: "
                      << p.id << "]" << std::endl;
        }
    } else {
        for (const Property &p: propertyByRegionId) {
            std::cout << "[ ( Region: " << p.region << ", ID: " << p.id << ") , City: " << p.city << ", ID: " << p.id
                      << "]" << std::endl;
        }
    }
}


CLandRegister::Property::Property(std::string city, std::string addr, std::string region, unsigned int id) : city(
        std::move(city)),
                                                                                                             addr(std::move(
                                                                                                                     addr)),
                                                                                                             region(std::move(
                                                                                                                     region)),
                                                                                                             id(id) {}
