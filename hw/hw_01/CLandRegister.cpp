#include "CLandRegister.hpp"

bool CLandRegister::add(const std::string &city, const std::string &addr, const std::string &region, unsigned int id) {
    if (isInProperty(city, addr) || isInProperty(region, id)) {
        return false;
    }

    auto *land = new CLand(city, addr, region, id);
    land->setOwner("");

    if (byOwner.empty()) {
        byOwner.push_back(new COwner(""));
    }

    auto it = std::lower_bound(byOwner[0]->owned.begin(), byOwner[0]->owned.end(), land,
                               cmpByRegion);

    land->acquire = ++byOwner[0]->acquiredCount;
    byOwner[0]->owned.insert(it, land);

    it = std::lower_bound(byRegionId.begin(), byRegionId.end(), land,
                          cmpByRegion);
    byRegionId.insert(it, land);

    it = std::lower_bound(byCityAddr.begin(), byCityAddr.end(), land,
                          cmpByCity);
    byCityAddr.insert(it, land);
    return true;
}

bool CLandRegister::del(const std::string &city, const std::string &addr) {
    CLand land = CLand(city, addr, "", 0);

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
    CLand land = CLand("", "", region, id);
    std::pair<std::string, std::vector<CLand *>> ownedLand;

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
    CLand land = CLand(city, addr, "", 0);

    auto it = std::lower_bound(byCityAddr.begin(), byCityAddr.end(), &land,
                               cmpByCity);
    if (it == byCityAddr.end() || (*it)->city != city || (*it)->addr != addr) {

        return false;
    }
    owner = (*it)->owner;
    return true;
}

bool CLandRegister::getOwner(const std::string &region, unsigned int id, std::string &owner) const {
    CLand land = CLand("", "", region, id);
    auto it = std::lower_bound(byRegionId.begin(), byRegionId.end(), &land,
                               cmpByRegion);
    if (it == byRegionId.end() || (*it)->region != region || (*it)->id != id) {
        return false;
    }
    owner = (*it)->owner;
    return true;
}

bool CLandRegister::addNewOwner(const std::string &owner, CLand *land) {
    COwner tmpOwner = COwner(owner);

    auto itOwner = std::lower_bound(byOwner.begin(), byOwner.end(), &tmpOwner, cmpByOwner);
    if (itOwner == byOwner.end() || (*itOwner)->nameLower != tmpOwner.nameLower) {
        auto *newOwner = new COwner(owner);
        land->acquire = ++newOwner->acquiredCount;
        newOwner->owned.push_back(land);
        byOwner.push_back(newOwner);
    } else {
        auto it = std::lower_bound((*itOwner)->owned.begin(), (*itOwner)->owned.end(), land,
                                   cmpByRegion);
        if ((*itOwner)->nameLower == tmpOwner.nameLower && (*it) == land) {
            return false;
        }

        land->acquire = ++(*itOwner)->acquiredCount;
        (*itOwner)->owned.insert(it, land);
    }
    removeOwner(land->owner, land);
    land->owner = owner;

    return true;
}

void CLandRegister::removeOwner(const std::string &owner, CLand *land) {
    COwner ownedLand = COwner(owner);

    auto itOwner = std::lower_bound(byOwner.begin(), byOwner.end(), &ownedLand, cmpByOwner);
    if (itOwner != byOwner.end() && (*itOwner)->nameLower == ownedLand.nameLower) {
        auto itLand = std::lower_bound((*itOwner)->owned.begin(), (*itOwner)->owned.end(),
                                       land, cmpByRegion);
        if (itLand != (*itOwner)->owned.end() && (*itLand)->region == land->region) {
            (*itOwner)->owned.erase(itLand);
        }
    }
}

bool CLandRegister::newOwner(const std::string &city, const std::string &addr, const std::string &owner) {
    CLand land = CLand(city, addr, "", 0);
    auto it = std::lower_bound(byCityAddr.begin(), byCityAddr.end(), &land,
                               cmpByCity);
    if (it == byCityAddr.end() || (*it)->city != city || (*it)->addr != addr) {
        return false;
    }

    return addNewOwner(owner, *it);
}

bool CLandRegister::newOwner(const std::string &region, unsigned int id, const std::string &owner) {
    CLand land = CLand("", "", region, id);

    auto it = std::lower_bound(byRegionId.begin(), byRegionId.end(), &land,
                               cmpByRegion);
    if (it == byRegionId.end() || (*it)->region != region || (*it)->id != id) {
        return false;
    }

    return addNewOwner(owner, *it);
}

size_t CLandRegister::count(const std::string &owner) const {
    COwner tmpOwner = COwner(owner);

    auto it = std::lower_bound(byOwner.begin(), byOwner.end(), &tmpOwner,
                               cmpByOwner);

    if (it == byOwner.end() || (*it)->nameLower != tmpOwner.nameLower) return 0;
    return (*it)->owned.size();
}

CIterator CLandRegister::listByAddr() const {
    return {byCityAddr};
}

CIterator CLandRegister::listByOwner(const std::string &owner) const {
    COwner tmpOwner = COwner(owner);

    auto it = std::lower_bound(byOwner.begin(), byOwner.end(), &tmpOwner,
                               cmpByOwner);
    if (it == byOwner.end() || (*it)->nameLower != tmpOwner.nameLower) {
        return {{}};
    }

    return {(*it)->getSortedCopyByAcquire()};
}


bool CLandRegister::isInProperty(const std::string &region, unsigned int id) {
    CLand land = CLand("", "", region, id);
    auto it = std::lower_bound(byRegionId.begin(), byRegionId.end(),
                               &land, CLandRegister::cmpByRegion);
    return (it != byRegionId.end() && region == (*it)->region && id == (*it)->id);
}

bool CLandRegister::isInProperty(const std::string &city, const std::string &addr) {
    CLand land = CLand(city, addr, "", 0);
    auto it = std::lower_bound(byCityAddr.begin(), byCityAddr.end(),
                               &land, CLandRegister::cmpByCity);
    return (it != byCityAddr.end() && city == (*it)->city && addr == (*it)->addr);
}

bool CLandRegister::cmpByCity(const CLand *p1, const CLand *p2) {
    return (p1->city < p2->city) || (p1->city == p2->city && p1->addr < p2->addr);
};


bool CLandRegister::cmpByRegion(const CLand *p1, const CLand *p2) {
    return (p1->region < p2->region) || (p1->region == p2->region && p1->id < p2->id);
}

bool CLandRegister::cmpByOwner(const COwner *o1,
                               const COwner *o2) {
    return (o1->nameLower < o2->nameLower);
}


void CLandRegister::print(const std::string &category) {
    if (category == "_c") {
        std::cout << "By City" << std::endl;
        for (const CLand *p: byCityAddr) {
            std::cout << "[ ( City: " << p->city << ", Addr: " << p->addr << ") , Region: " << p->region << ", ID: "
                      << p->id << ", Owner: " << p->owner << " ]" << std::endl;
        }
    } else if (category == "_a") {
        std::cout << "By Region" << std::endl;
        for (const CLand *p: byRegionId) {
            std::cout << "[ ( Region: " << p->region << ", ID: " << p->id << ") , City: " << p->city << ", Addr: "
                      << p->addr << ", Owner: " << p->owner << " ]" << std::endl;
        }
    } else if (category == "_o") {
        std::cout << "By Owner" << std::endl;
        for (COwner *owner: byOwner) {
            std::cout << "Owner: " << owner->nameLower << std::endl;
            for (const CLand *p: owner->owned) {
                std::cout << "[ ( Region: " << p->region << ", ID: " << p->id << ") , City: " << p->city << ", Addr: "
                          << p->addr << ", Owner: " << p->owner << " ] Acquire: " << p->acquire << std::endl;
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


CLandRegister::~CLandRegister() {
    for (COwner *owner: byOwner) {
        delete owner;
    }
    for (CLand *land: byRegionId) {
        delete land;
    }
}
