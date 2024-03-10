#include "CLandRegister.hpp"

bool CLandRegister::add(const std::string &city, const std::string &addr, const std::string &region, unsigned int id) {
    if (isInProperty(city, addr) || isInProperty(region, id)) {
        return false;
    }

    Land land = Land(city, addr, region, id);
    land.setOwner("");

    if(byOwner.empty()){
        std::pair<std::string, std::vector<Land>> state;
        state.first = "";
        byOwner.push_back(state);
    }

    byOwner[0].second.push_back(land);
    auto insertPointRegion = std::upper_bound(byRegionId.begin(), byRegionId.end(), land,
                                              cmpByRegion);
    byRegionId.insert(insertPointRegion, land);

    auto insertPointAddr = std::upper_bound(byCityAddr.begin(), byCityAddr.end(), land,
                                            cmpByCity);
    byCityAddr.insert(insertPointAddr, land);
    return true;
}

bool CLandRegister::del(const std::string &city, const std::string &addr) {
    Land land = Land(city, addr, "", 0);
    auto boundsCity = std::equal_range(byCityAddr.begin(), byCityAddr.end(), land,
                                       cmpByCity);
    if (boundsCity.first == boundsCity.second) {
        return false;
    }

    land.setOwner(boundsCity.first->owner);
    land.region = boundsCity.first->region;
    land.id = boundsCity.first->id;

    byCityAddr.erase(boundsCity.first, boundsCity.second);
    auto boundsRegion = std::equal_range(byRegionId.begin(), byRegionId.end(), land,
                                         cmpByRegion);

    byRegionId.erase(boundsRegion.first, boundsRegion.second);
    removeOwner(land.owner, land);

    return true;
}

bool CLandRegister::del(const std::string &region, unsigned int id) {
    Land land = Land("", "", region, id);

    auto boundsRegion = std::equal_range(byRegionId.begin(), byRegionId.end(), land,
                                         cmpByRegion);
    if (boundsRegion.first == boundsRegion.second) {
        return false;
    }

    land.setOwner(boundsRegion.first->owner);
    land.city = boundsRegion.first->city;
    land.addr = boundsRegion.second->addr;

    byRegionId.erase(boundsRegion.first, boundsRegion.second);

    auto boundsCity = std::equal_range(byCityAddr.begin(), byCityAddr.end(), land,
                                       cmpByCity);

    byCityAddr.erase(boundsCity.first, boundsCity.second);
    removeOwner(land.owner, land);

    return true;
}

bool CLandRegister::getOwner(const std::string &city, const std::string &addr, std::string &owner) const {
    auto bounds = std::equal_range(byCityAddr.begin(), byCityAddr.end(), Land(city, addr, "", 0),
                                   cmpByCity);
    if (bounds.first == bounds.second) {
        return false;
    }
    owner = bounds.first->owner;
    return true;
}

bool CLandRegister::getOwner(const std::string &region, unsigned int id, std::string &owner) const {
    auto bounds = std::equal_range(byRegionId.begin(), byRegionId.end(), Land("", "", region, id),
                                   cmpByRegion);
    if (bounds.first == bounds.second) {
        return false;
    }
    owner = bounds.first->owner;
    return true;
}

void CLandRegister::addNewOwner(const std::string &owner, const Land &land) {
    std::pair<std::string, std::vector<Land>> ownedLand;
    ownedLand.first = stringToLower(owner);
    auto boundsOwner = std::equal_range(byOwner.begin(), byOwner.end(), ownedLand, cmpByOwner);
    if (boundsOwner.first == boundsOwner.second) {
        ownedLand.second.push_back(land);
        byOwner.push_back(ownedLand);
    } else {
        boundsOwner.first->second.push_back(land);
    }
    removeOwner(land.owner, land);
}

void CLandRegister::removeOwner(const std::string &owner, const Land &land) {
    std::pair<std::string, std::vector<Land>> ownedLand;
    ownedLand.first = stringToLower(owner);
    auto boundsOwner = std::equal_range(byOwner.begin(), byOwner.end(), ownedLand, cmpByOwner);
    if (boundsOwner.first != boundsOwner.second) {
        auto boundsLand = std::equal_range(boundsOwner.first->second.begin(), boundsOwner.first->second.end(),
                                           land, cmpByCity);
        if(boundsLand.first != boundsLand.second){
            boundsOwner.first->second.erase(boundsLand.first,boundsLand.second);
        }

    }
}

bool CLandRegister::newOwner(const std::string &city, const std::string &addr, const std::string &owner) {
    Land land = Land(city, addr, "", 0);
    auto boundsCity = std::equal_range(byCityAddr.begin(), byCityAddr.end(), land,
                                       cmpByCity);
    if (boundsCity.first == boundsCity.second) {
        return false;
    }
    std::string lowerOwner = stringToLower(owner);

    land.owner = boundsCity.first->owner;
    land.region = boundsCity.first->region;
    land.id = boundsCity.first->id;
    land.city = boundsCity.first->city;
    land.addr = boundsCity.first->addr;

    auto boundsRegion = std::equal_range(byRegionId.begin(), byRegionId.end(), Land(city, addr, "", 0),
                                         cmpByRegion);
    boundsRegion.first->setOwner(lowerOwner);
    boundsCity.first->setOwner(lowerOwner);

    addNewOwner(owner, land);
    return true;
}

bool CLandRegister::newOwner(const std::string &region, unsigned int id, const std::string &owner) {
    Land land = Land("", "", region, id);

    auto boundsRegion = std::equal_range(byRegionId.begin(), byRegionId.end(), land,
                                         cmpByRegion);
    if (boundsRegion.first == boundsRegion.second) {
        return false;
    }

    land.owner = boundsRegion.first->owner;
    land.region = boundsRegion.first->region;
    land.id = boundsRegion.first->id;
    land.city = boundsRegion.first->city;
    land.addr = boundsRegion.first->addr;

    auto boundsCity = std::equal_range(byCityAddr.begin(), byCityAddr.end(), land,
                                       cmpByCity);
    boundsCity.first->setOwner(stringToLower(owner));
    boundsRegion.first->setOwner(stringToLower(owner));

    addNewOwner(owner, land);
    return true;
}

size_t CLandRegister::count(const std::string &owner) const {
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
    std::pair<std::string, std::vector<Land>> ownedLand;
    ownedLand.first = owner;
    auto boundsOwner = std::equal_range(byOwner.begin(), byOwner.end(), ownedLand,
                                        [](const std::pair<std::string, std::vector<Land>> &l1,
                                           const std::pair<std::string, std::vector<Land>> &l2) {
                                            return l1.first < l2.first;
                                        });
    if (boundsOwner.first == boundsOwner.second) {
        return {{}};
    }

    std::vector<Land> ownedLandVector = boundsOwner.first->second;
    return {std::move(ownedLandVector)};
}


bool CLandRegister::isInProperty(const std::string &region, unsigned int id) {
    auto it = std::equal_range(byRegionId.begin(), byRegionId.end(),
                               Land("", "", region, id), CLandRegister::cmpByRegion);
    return it.first != it.second;
}

bool CLandRegister::isInProperty(const std::string &city, const std::string &addr) {
    auto it = std::equal_range(byRegionId.begin(), byRegionId.end(),
                               Land(city, addr, "", 0), CLandRegister::cmpByCity);
    return it.first != it.second;
}

bool CLandRegister::cmpByCity(const Land &p1, const Land &p2) {
    return (p1.city < p2.city) || (p1.city == p2.city && p1.addr < p2.addr);
};


bool CLandRegister::cmpByRegion(const Land &p1, const Land &p2) {
    return (p1.region < p2.region) || (p1.region == p2.region && p1.id < p2.id);
}

bool CLandRegister::cmpByOwner(const std::pair<std::string, std::vector<Land>> &l1,
                               const std::pair<std::string, std::vector<Land>> &l2) {
    return (l1.first < l2.first);
}


void CLandRegister::print(const std::string &category) {
    if (category == "_c") {
        std::cout << "By City" << std::endl;
        for (const Land &p: byCityAddr) {
            std::cout << "[ ( City: " << p.city << ", Addr: " << p.addr << ") , Region: " << p.region << ", ID: "
                      << p.id << ", Owner: " << p.owner << " ]" << std::endl;
        }
    } else if (category == "_a") {
        std::cout << "By Region" << std::endl;
        for (const Land &p: byRegionId) {
            std::cout << "[ ( Region: " << p.region << ", ID: " << p.id << ") , City: " << p.city << ", Addr: "
                      << p.addr << ", Owner: " << p.owner <<" ]" << std::endl;
        }
    } else if (category == "_o") {
        std::cout << "By Owner" << std::endl;
        for (std::pair<std::string, std::vector<Land>> &owner: byOwner) {
            std::cout << "Owner: "<< owner.first << std::endl;
            for (const Land &p : owner.second) {
                std::cout << "[ ( Region: " << p.region << ", ID: " << p.id << ") , City: " << p.city << ", Addr: "
                          << p.addr << ", Owner: " << p.owner << " ]" << std::endl;
            }
        }
    } else {
        std::cout << "By Iterator" << std::endl;
        for (CIterator p = listByOwner(category); !p.atEnd(); p.next()) {
            std::cout << "[ ( Owner: " << p.owner()  << ") , City: " << p.city()
                      << ", Addr: "
                      << p.addr() << ", Region: " << p.region() << ", ID: " << p.id()
                      << " ]" << std::endl;
        }
    }
    std::cout << "---" << std::endl;
}

std::string CLandRegister::stringToLower(const std::string &in) {
    std::string out;
    for (char c: in) {
        out += static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return out;
}