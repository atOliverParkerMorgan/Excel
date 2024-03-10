#include "CLandRegister.hpp"

bool CLandRegister::add(const std::string &city, const std::string &addr, const std::string &region, unsigned int id) {
    if (isInProperty(city, addr) || isInProperty(region, id)) {
        return false;
    }

    Land land = Land(city, addr, region, id, INT16_MAX);
    land.setOwner("");

    auto insertPointOwner = std::lower_bound(byOwner.begin(), byOwner.end(), land,
                                             cmpByOwner);
    if (insertPointOwner == byOwner.begin()) {
        land.acquireOrder = 0;
    } else {
        auto last = (insertPointOwner - 1);
        if (last->owner == land.owner) {
            land.acquireOrder = last->acquireOrder + 1;
        } else {
            land.acquireOrder = 0;
        }
    }

    byOwner.insert(insertPointOwner, land);

    auto insertPointRegion = std::upper_bound(byRegionId.begin(), byRegionId.end(), land,
                                              cmpByRegion);
    byRegionId.insert(insertPointRegion, land);

    auto insertPointAddr = std::upper_bound(byCityAddr.begin(), byCityAddr.end(), land,
                                            cmpByCity);
    byCityAddr.insert(insertPointAddr, land);
    return true;
}

bool CLandRegister::del(const std::string &city, const std::string &addr) {
    Land land = Land(city, addr, "", 0, 0);
    auto boundsCity = std::equal_range(byCityAddr.begin(), byCityAddr.end(), land,
                                       cmpByCity);
    if (boundsCity.first == boundsCity.second) {
        return false;
    }

    land.setOwner(boundsCity.first->owner);
    land.acquireOrder = boundsCity.first->acquireOrder;
    land.region = boundsCity.first->region;
    land.id = boundsCity.first->id;

    byCityAddr.erase(boundsCity.first, boundsCity.second);


    auto boundsOwner = std::equal_range(byOwner.begin(), byOwner.end(), land,
                                        cmpByOwner);
    byOwner.erase(boundsOwner.first, boundsOwner.second);

    auto boundsRegion = std::equal_range(byRegionId.begin(), byRegionId.end(), land,
                                         cmpByRegion);
    byRegionId.erase(boundsRegion.first, boundsRegion.second);


    return true;
}

bool CLandRegister::del(const std::string &region, unsigned int id) {
    Land land = Land("", "", region, id, 0);

    auto boundsRegion = std::equal_range(byRegionId.begin(), byRegionId.end(), land,
                                         cmpByRegion);
    if (boundsRegion.first == boundsRegion.second) {
        return false;
    }

    land.setOwner(boundsRegion.first->owner);
    land.city = boundsRegion.first->city;
    land.addr = boundsRegion.second->addr;
    land.acquireOrder = boundsRegion.first->acquireOrder;

    byRegionId.erase(boundsRegion.first, boundsRegion.second);


    auto boundsOwner = std::equal_range(byOwner.begin(), byOwner.end(), land,
                                        cmpByOwner);
    byOwner.erase(boundsOwner.first, boundsOwner.second);

    auto boundsCity = std::equal_range(byCityAddr.begin(), byCityAddr.end(), land,
                                       cmpByCity);
    byCityAddr.erase(boundsCity.first, boundsCity.second);


    return true;
}

bool CLandRegister::getOwner(const std::string &city, const std::string &addr, std::string &owner) const {
    auto bounds = std::equal_range(byOwner.begin(), byCityAddr.end(), Land(city, addr, "", 0, 0),
                                   cmpByCity);
    if (bounds.first == bounds.second) {
        return false;
    }
    owner = bounds.first->owner;
    return true;
}

bool CLandRegister::getOwner(const std::string &region, unsigned int id, std::string &owner) const {
    auto bounds = std::equal_range(byOwner.begin(), byRegionId.end(), Land("", "", region, id, 0),
                                   cmpByRegion);
    if (bounds.first == bounds.second) {
        return false;
    }
    owner = bounds.first->owner;
    return true;
}

bool CLandRegister::newOwner(const std::string &city, const std::string &addr, const std::string &owner) {
    auto bounds = std::equal_range(byCityAddr.begin(), byCityAddr.end(), Land(city, addr, "", 0, 0),
                                   cmpByCity);
    if (bounds.first == bounds.second) {
        return false;
    }
    bounds.first->setOwner(owner);
    return true;
}

bool CLandRegister::newOwner(const std::string &region, unsigned int id, const std::string &owner) {
    auto bounds = std::equal_range(byRegionId.begin(), byRegionId.end(), Land("", "", region, id, 0),
                                   cmpByRegion);
    if (bounds.first == bounds.second) {
        return false;
    }
    bounds.first->setOwner(owner);
    return true;
}

size_t CLandRegister::count(const std::string &owner) const {
//    Land land = Land("", "", "", 0, 0);
//    land.setOwner(owner);
//    land.acquireOrder = -1;
//    auto bounds_lower = std::lower_bound(byRegionId.begin(), byRegionId.end(), land,
//                                   cmpByOwner);
//
//
//
//    return bounds.;
    size_t cnt = 0;
    for (const Land &p: byRegionId) {
        if (p.owner == owner) ++cnt;
    }

    return cnt;
}

CIterator CLandRegister::listByAddr() const {
    return {byCityAddr};
}

CIterator CLandRegister::listByOwner(const std::string &owner) const {
    Land land = Land("", "", "", 0, 0);
    land.setOwner(owner);
    auto boundsOwner = std::equal_range(byOwner.begin(), byOwner.end(), land,
                                        cmpByOwner);

    std::vector<Land> tmp(boundsOwner.first, boundsOwner.second);

    return {std::move(tmp)};
}


bool CLandRegister::isInProperty(const std::string &region, unsigned int id) {
    auto it = std::equal_range(byRegionId.begin(), byRegionId.end(),
                               Land("", "", region, id, 0), CLandRegister::cmpByRegion);
    return it.first != it.second;
}

bool CLandRegister::isInProperty(const std::string &city, const std::string &addr) {
    auto it = std::equal_range(byRegionId.begin(), byRegionId.end(),
                               Land(city, addr, "", 0, 0), CLandRegister::cmpByCity);
    return it.first != it.second;
}

bool CLandRegister::cmpByCity(const Land &p1, const Land &p2) {
    return (p1.city < p2.city) || (p1.city == p2.city && p1.addr < p2.addr);
};


bool CLandRegister::cmpByRegion(const Land &p1, const Land &p2) {
    return (p1.region < p2.region) || (p1.region == p2.region && p1.id < p2.id);
}

bool CLandRegister::cmpByOwner(const Land &p1, const Land &p2) {
    return (p1.ownerLower < p2.ownerLower) || (p1.ownerLower == p2.ownerLower && p1.acquireOrder < p2.acquireOrder);
}


void CLandRegister::print(std::string category) {
    if (category == "_c") {
        std::cout << "By City" << std::endl;
        for (const Land &p: byCityAddr) {
            std::cout << "[ ( City: " << p.city << ", Addr: " << p.addr << ") , Region: " << p.region << ", ID: "
                      << p.id << ", Owner: " << p.owner << ", Acquire: " << p.acquireOrder << " ]" << std::endl;
        }
    } else if (category == "_a") {
        std::cout << "By Region" << std::endl;
        for (const Land &p: byRegionId) {
            std::cout << "[ ( Region: " << p.region << ", ID: " << p.id << ") , City: " << p.city << ", Addr: "
                      << p.addr << ", Owner: " << p.owner << ", Acquire: " << p.acquireOrder << " ]" << std::endl;
        }
    } else if(category == "_o"){
        std::cout << "By Owner" << std::endl;
        for (const Land &p: byOwner) {
            std::cout << "[ ( Owner: " << p.owner << ", Acquire: " << p.acquireOrder << ") , City: " << p.city
                      << ", Addr: "
                      << p.addr << ", Region: " << p.region << ", ID: " << p.id
                      << " ]" << std::endl;
        }
    } else{
        std::cout << "By Iterator" << std::endl;
        for ( CIterator p = listByOwner(category); !p.atEnd(); p.next()) {
            std::cout << "[ ( Owner: " << p.owner() << ", Acquire: " << "null" << ") , City: " << p.city()
                      << ", Addr: "
                      << p.addr() << ", Region: " << p.region() << ", ID: " << p.id()
                      << " ]" << std::endl;
        }
    }
    std::cout << "---" << std::endl;
}


