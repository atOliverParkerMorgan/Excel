#include "CLandRegister.hpp"

bool CLandRegister::add(const std::string &city, const std::string &addr, const std::string &region, unsigned int id) {
    if (isInProperty(city, addr) || isInProperty(region, id)) {
        return false;
    }

    Land *land = new Land(city, addr, region, id);
    land->setOwner("");

    if (byOwner.empty()) {
        std::pair<std::string, std::vector<Land *>> state;
        state.first = "";
        byOwner.push_back(state);
    }

    auto it = std::lower_bound(byOwner[0].second.begin(), byOwner[0].second.end(), land,
                               cmpByRegion);
    byOwner[0].second.insert(it, land);

    it = std::lower_bound(byRegionId.begin(), byRegionId.end(), land,
                          cmpByRegion);
    byRegionId.insert(it, land);

    it = std::lower_bound(byCityAddr.begin(), byCityAddr.end(), land,
                          cmpByCity);
    byCityAddr.insert(it, land);
    return true;
}

bool CLandRegister::del(const std::string &city, const std::string &addr) {
    Land land = Land(city, addr, "", 0);

    auto it = std::lower_bound(byCityAddr.begin(), byCityAddr.end(), &land, cmpByCity);
    if (it == byCityAddr.end() || (*it)->city != city || (*it)->addr != addr) {
        return false;
    }

    land.region = (*it)->region;
    land.id = (*it)->id;
    removeOwner((*it)->owner, &land);
    byCityAddr.erase(it);


    it = std::lower_bound(byRegionId.begin(), byRegionId.end(), &land, cmpByRegion);
    if (it != byRegionId.end() && (*it)->region == land.region && (*it)->id == land.id) {
        byRegionId.erase(it);
    }


    return true;
}

bool CLandRegister::del(const std::string &region, unsigned int id) {
    Land land = Land("", "", region, id);
    std::pair<std::string, std::vector<Land *>> ownedLand;

    auto it = std::lower_bound(byRegionId.begin(), byRegionId.end(), &land, cmpByRegion);
    if (it == byRegionId.end() || (*it)->region != region || (*it)->id != id) {
        return false;
    }

    land.city = (*it)->city;
    land.addr = (*it)->addr;
    removeOwner((*it)->owner, &land);
    byRegionId.erase(it);


    it = std::lower_bound(byCityAddr.begin(), byCityAddr.end(), &land, cmpByCity);
    byCityAddr.erase(it);

    return true;
}

bool CLandRegister::getOwner(const std::string &city, const std::string &addr, std::string &owner) const {
    Land land = Land(city, addr, "", 0);

    auto bounds = std::lower_bound(byCityAddr.begin(), byCityAddr.end(), &land,
                                   cmpByCity);
    if (bounds == byCityAddr.end()) {
        return false;
    }
    owner = (*bounds)->owner;
    return true;
}

bool CLandRegister::getOwner(const std::string &region, unsigned int id, std::string &owner) const {
    Land land = Land("", "", region, id);
    auto bounds = std::lower_bound(byRegionId.begin(), byRegionId.end(), &land,
                                   cmpByRegion);
    if (bounds == byRegionId.end()) {
        return false;
    }
    owner = (*bounds)->owner;
    return true;
}

void CLandRegister::addNewOwner(const std::string &owner, Land *land) {
    std::pair<std::string, std::vector<Land *>> ownedLand;
    ownedLand.first = stringToLower(owner);
    auto itOwner = std::lower_bound(byOwner.begin(), byOwner.end(), ownedLand, cmpByOwner);
    if (itOwner == byOwner.end()) {
        ownedLand.second.push_back(land);
        byOwner.push_back(ownedLand);
    } else {
        auto it = std::lower_bound(itOwner->second.begin(), itOwner->second.end(), land,
                                   cmpByRegion);
        itOwner->second.insert(it, land);
    }
    removeOwner(land->owner, land);
}

void CLandRegister::removeOwner(const std::string &owner, Land *land) {
    std::pair<std::string, std::vector<Land *>> ownedLand;
    ownedLand.first = stringToLower(owner);
    auto itOwner = std::lower_bound(byOwner.begin(), byOwner.end(), ownedLand, cmpByOwner);
    if (itOwner != byOwner.end() || itOwner->first != owner) {
        auto itLand = std::lower_bound(itOwner->second.begin(), itOwner->second.end(),
                                       land, cmpByRegion);
        if (itLand != itOwner->second.end()) {
            itOwner->second.erase(itLand);
        }

    }
}

bool CLandRegister::newOwner(const std::string &city, const std::string &addr, const std::string &owner) {
    Land land = Land(city, addr, "", 0);
    auto boundsCity = std::lower_bound(byCityAddr.begin(), byCityAddr.end(), &land,
                                       cmpByCity);
    if (boundsCity == byCityAddr.end()) {
        return false;
    }
    std::string lowerOwner = stringToLower(owner);
    addNewOwner(owner, &land);

    return true;
}

bool CLandRegister::newOwner(const std::string &region, unsigned int id, const std::string &owner) {
    Land land = Land("", "", region, id);

    auto boundsRegion = std::lower_bound(byRegionId.begin(), byRegionId.end(), &land,
                                         cmpByRegion);
    if (boundsRegion == byRegionId.end()) {
        return false;
    }

    return true;
}

size_t CLandRegister::count(const std::string &owner) const {
    std::pair<std::string, std::vector<Land *>> ownedLand;
    ownedLand.first = owner;
    auto boundsOwner = std::lower_bound(byOwner.begin(), byOwner.end(), ownedLand,
                                        cmpByOwner);

    if (boundsOwner == byOwner.end()) return 0;
    return boundsOwner->second.size();
}

CIterator CLandRegister::listByAddr() const {
    return {byCityAddr};
}

CIterator CLandRegister::listByOwner(const std::string &owner) const {
    std::pair<std::string, std::vector<Land *>> ownedLand;
    ownedLand.first = owner;
    auto boundsOwner = std::lower_bound(byOwner.begin(), byOwner.end(), ownedLand,
                                        cmpByOwner);
    if (boundsOwner == byOwner.end()) {
        return {{}};
    }

    std::vector<Land *> ownedLandVector = boundsOwner->second;
    return {std::move(ownedLandVector)};
}


bool CLandRegister::isInProperty(const std::string &region, unsigned int id) {
    Land land = Land("", "", region, id);
    auto it = std::lower_bound(byRegionId.begin(), byRegionId.end(),
                               &land, CLandRegister::cmpByRegion);
    return (it != byRegionId.end() && !cmpByRegion(&land, *it));
}

bool CLandRegister::isInProperty(const std::string &city, const std::string &addr) {
    Land land = Land(city, addr, "", 0);
    auto it = std::lower_bound(byCityAddr.begin(), byCityAddr.end(),
                               &land, CLandRegister::cmpByCity);
    return (it != byCityAddr.end() && !cmpByCity(&land, *it));
}

bool CLandRegister::cmpByCity(const Land *p1, const Land *p2) {
    return (p1->city < p2->city) || (p1->city == p2->city && p1->addr < p2->addr);
};


bool CLandRegister::cmpByRegion(const Land *p1, const Land *p2) {
    return (p1->region < p2->region) || (p1->region == p2->region && p1->id < p2->id);
}

bool CLandRegister::cmpByOwner(const std::pair<std::string, std::vector<Land *>> &l1,
                               const std::pair<std::string, std::vector<Land *>> &l2) {
    return (l1.first < l2.first);
}


void CLandRegister::print(const std::string &category) {
    if (category == "_c") {
        std::cout << "By City" << std::endl;
        for (const Land *p: byCityAddr) {
            std::cout << "[ ( City: " << p->city << ", Addr: " << p->addr << ") , Region: " << p->region << ", ID: "
                      << p->id << ", Owner: " << p->owner << " ]" << std::endl;
        }
    } else if (category == "_a") {
        std::cout << "By Region" << std::endl;
        for (const Land *p: byRegionId) {
            std::cout << "[ ( Region: " << p->region << ", ID: " << p->id << ") , City: " << p->city << ", Addr: "
                      << p->addr << ", Owner: " << p->owner << " ]" << std::endl;
        }
    } else if (category == "_o") {
        std::cout << "By Owner" << std::endl;
        for (std::pair<std::string, std::vector<Land *>> &owner: byOwner) {
            std::cout << "Owner: " << owner.first << std::endl;
            for (const Land *p: owner.second) {
                std::cout << "[ ( Region: " << p->region << ", ID: " << p->id << ") , City: " << p->city << ", Addr: "
                          << p->addr << ", Owner: " << p->owner << " ]" << std::endl;
            }
        }
    } else {
        std::cout << "By Iterator" << std::endl;
        for (CIterator p = listByOwner(category); !p.atEnd(); p.next()) {
            std::cout << "[ ( Owner: " << p.owner() << ") , City: " << p.city()
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

CLandRegister::~CLandRegister() {
    for (Land *land: byRegionId) {
        delete land;
    }
}
