#include "CLandRegister.hpp"

#include <utility>

bool CLandRegister::add(const std::string &city, const std::string &addr, const std::string &region, unsigned int id) {
    if (isInProperty(city, addr) || isInProperty(region, id)) {
        return false;
    }
    const Property property = Property(city, addr, region, id);
    auto insertPointRegion = std::upper_bound(byRegionId.begin(), byRegionId.end(), property,
                                              cmpByRegion);
    byRegionId.insert(insertPointRegion, property);

    auto insertPointAddr = std::upper_bound(byCityAddr.begin(), byCityAddr.end(), property,
                                            cmpByCity);
    byCityAddr.insert(insertPointAddr, property);
    return true;
}

bool CLandRegister::del(const std::string &city, const std::string &addr) {
    auto bounds = std::equal_range(byCityAddr.begin(), byCityAddr.end(), Property(city, addr, "", 0),
                                   cmpByCity);
    if (bounds.first == bounds.second) {
        return false;
    }
    byCityAddr.erase(bounds.first, bounds.second);
    return true;
}

bool CLandRegister::del(const std::string &region, unsigned int id) {
    auto bounds = std::equal_range(byRegionId.begin(), byRegionId.end(), Property("", "", region, id),
                                   cmpByRegion);
    if (bounds.first == bounds.second) {
        return false;
    }
    byRegionId.erase(bounds.first, bounds.second);
    return true;
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


bool CLandRegister::isInProperty(const std::string &region, unsigned int id) {
    auto it = std::equal_range(byRegionId.begin(), byRegionId.end(),
                               Property("", "", region, id), CLandRegister::cmpByRegion);
    return it.first != it.second;
}

bool CLandRegister::isInProperty(const std::string &city, const std::string &addr) {
    auto it = std::equal_range(byRegionId.begin(), byRegionId.end(),
                               Property(city, addr, "", 0), CLandRegister::cmpByCity);
    return it.first != it.second;
}

bool CLandRegister::cmpByCity(const CLandRegister::Property &p1, const CLandRegister::Property &p2) {
    return (p1.city < p2.city) || (p1.city == p2.city && p1.addr < p2.addr);
};


bool CLandRegister::cmpByRegion(const CLandRegister::Property &p1, const CLandRegister::Property &p2) {
    return (p1.region < p2.region) || (p1.region == p2.region && p1.id < p2.id);
}

void CLandRegister::print(bool byAddr) {
    if (byAddr) {
        std::cout << "By City" << std::endl;
        for (const Property &p: byCityAddr) {
            std::cout << "[ ( City: " << p.city << ", Addr: " << p.addr << ") , Region: " << p.region << ", ID: "
                      << p.id << " ]" << std::endl;
        }
    } else {
        std::cout << "By Region" << std::endl;
        for (const Property &p: byRegionId) {
            std::cout << "[ ( Region: " << p.region << ", ID: " << p.id << ") , City: " << p.city << ", Addr: "
                      << p.addr
                      << " ]" << std::endl;
        }
    }
    std::cout << "---" << std::endl;
}


CLandRegister::Property::Property(std::string city, std::string addr, std::string region, unsigned int id) : city(
        std::move(city)),
                                                                                                             addr(std::move(
                                                                                                                     addr)),
                                                                                                             region(std::move(
                                                                                                                     region)),
                                                                                                             id(id) {}
