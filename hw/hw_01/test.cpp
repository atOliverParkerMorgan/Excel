#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>

#endif /* __PROGTEST__ */

struct CLand {

    CLand(std::string city,
          std::string addr,
          std::string region,
          unsigned int id);

    std::string city;
    std::string addr;
    std::string region;
    unsigned int id;
    std::string owner;
    size_t acquire;

    bool operator==(const CLand &other) const;

    void setOwner(const std::string &newOwner);
};

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


class COwner {
public:
    explicit COwner(const std::string &name);

    std::string nameLower;
    size_t acquiredCount;
    std::vector<CLand *> owned;

    static std::string stringToLower(const std::string &in);

    std::vector<CLand *> getSortedCopyByAcquire() const;
};

COwner::COwner(const std::string &name) : nameLower(std::move(stringToLower(name))), acquiredCount(0) {}

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


class CIterator {
private:
    std::vector<CLand *> lands;
    size_t currentIndex;
public:
    CIterator(std::vector<CLand *> in_land);

    bool atEnd() const;

    void next();

    std::string city() const;

    std::string addr() const;

    std::string region() const;

    unsigned id() const;

    std::string owner() const;
};


bool CIterator::atEnd() const {
    return currentIndex == lands.size();
}

void CIterator::next() {
    if (!atEnd()) {
        currentIndex++;
    }
}

std::string CIterator::city() const {
    if (!atEnd()) {
        return lands[currentIndex]->city;
    }
    return "";
}

std::string CIterator::addr() const {
    if (!atEnd()) {
        return lands[currentIndex]->addr;
    }
    return "";
}

unsigned CIterator::id() const {
    if (!atEnd()) {
        return lands[currentIndex]->id;
    }
    return 0;
}

std::string CIterator::owner() const {
    if (!atEnd()) {
        return lands[currentIndex]->owner;
    }
    return "";
}

CIterator::CIterator(std::vector<CLand *> in_land) {
    this->lands = std::move(in_land);
    currentIndex = 0;
}

std::string CIterator::region() const {
    if (!atEnd()) {
        return lands[currentIndex]->region;
    }
    return "";
}

class CLandRegister {
public:
    CLandRegister() = default;

    ~CLandRegister();

    void print(const std::string &category);

    bool add(const std::string &city,
             const std::string &addr,
             const std::string &region,
             unsigned int id);

    bool del(const std::string &city,
             const std::string &addr);

    bool del(const std::string &region,
             unsigned int id);

    bool getOwner(const std::string &city,
                  const std::string &addr,
                  std::string &owner) const;

    bool getOwner(const std::string &region,
                  unsigned int id,
                  std::string &owner) const;

    bool newOwner(const std::string &city,
                  const std::string &addr,
                  const std::string &owner);

    bool newOwner(const std::string &region,
                  unsigned int id,
                  const std::string &owner);

    size_t count(const std::string &owner) const;

    CIterator listByAddr() const;

    CIterator listByOwner(const std::string &owner) const;


private:
    static bool cmpByRegion(const CLand *p1, const CLand *p2);

    static bool cmpByCity(const CLand *p1, const CLand *p2);

    static bool cmpByOwner(const COwner *o1,
                           const COwner *o2);

    bool isInProperty(const std::string &city,
                      const std::string &addr);

    bool isInProperty(const std::string &region,
                      unsigned int id);

    std::vector<CLand *> byCityAddr;
    std::vector<CLand *> byRegionId;
    std::vector<COwner *> byOwner;

    bool addNewOwner(const std::string &owner, CLand *land);

    void removeOwner(const std::string &owner, CLand *land);
};

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
    delete *it;

    byRegionId.erase(it);

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
    delete *it;
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

        auto it = std::lower_bound(byOwner.begin(), byOwner.end(), newOwner, cmpByOwner);
        byOwner.insert(it, newOwner);
    } else {
        auto it = std::lower_bound((*itOwner)->owned.begin(), (*itOwner)->owned.end(), land,
                                   cmpByRegion);
        if (it != (*itOwner)->owned.end() && (*itOwner)->nameLower == tmpOwner.nameLower && (*it) == land) {
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
        std::cout << "By CIterator" << std::endl;
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

        for (const CLand *land: owner->owned) {
            delete land;
        }
        delete owner;
    }
}


#ifndef __PROGTEST__

static void test0() {
    CLandRegister x;
    std::string owner;

    assert (x.add("Prague", "Thakurova", "Dejvice", 12345));
    assert (x.add("Prague", "Evropska", "Vokovice", 12345));
    assert (x.add("Prague", "Technicka", "Dejvice", 9873));
    assert (x.add("Plzen", "Evropska", "Plzen mesto", 78901));
    assert (x.add("Liberec", "Evropska", "Librec", 4552));
    CIterator i0 = x.listByAddr();
    assert (!i0.atEnd()
            && i0.city() == "Liberec"
            && i0.addr() == "Evropska"
            && i0.region() == "Librec"
            && i0.id() == 4552
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Plzen"
            && i0.addr() == "Evropska"
            && i0.region() == "Plzen mesto"
            && i0.id() == 78901
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Evropska"
            && i0.region() == "Vokovice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Technicka"
            && i0.region() == "Dejvice"
            && i0.id() == 9873
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Thakurova"
            && i0.region() == "Dejvice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (i0.atEnd());

    assert (x.count("") == 5);
    CIterator i1 = x.listByOwner("");
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Thakurova"
            && i1.region() == "Dejvice"
            && i1.id() == 12345
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Evropska"
            && i1.region() == "Vokovice"
            && i1.id() == 12345
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Technicka"
            && i1.region() == "Dejvice"
            && i1.id() == 9873
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Plzen"
            && i1.addr() == "Evropska"
            && i1.region() == "Plzen mesto"
            && i1.id() == 78901
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Liberec"
            && i1.addr() == "Evropska"
            && i1.region() == "Librec"
            && i1.id() == 4552
            && i1.owner() == "");
    i1.next();
    assert (i1.atEnd());

    assert (x.count("CVUT") == 0);
    CIterator i2 = x.listByOwner("CVUT");
    assert (i2.atEnd());

    assert (x.newOwner("Prague", "Thakurova", "CVUT"));
    assert (x.newOwner("Dejvice", 9873, "CVUT"));
    assert (x.newOwner("Plzen", "Evropska", "Anton Hrabis"));
    assert (x.newOwner("Librec", 4552, "Cvut"));
    x.getOwner("Prague", "Thakurova", owner);
    assert (x.getOwner("Prague", "Thakurova", owner) && owner == "CVUT");

    assert (x.getOwner("Dejvice", 12345, owner) && owner == "CVUT");
    assert (x.getOwner("Prague", "Evropska", owner) && owner == "");
    assert (x.getOwner("Vokovice", 12345, owner) && owner == "");
    assert (x.getOwner("Prague", "Technicka", owner) && owner == "CVUT");
    assert (x.getOwner("Dejvice", 9873, owner) && owner == "CVUT");
    assert (x.getOwner("Plzen", "Evropska", owner) && owner == "Anton Hrabis");
    assert (x.getOwner("Plzen mesto", 78901, owner) && owner == "Anton Hrabis");
    assert (x.getOwner("Liberec", "Evropska", owner) && owner == "Cvut");
    assert (x.getOwner("Librec", 4552, owner) && owner == "Cvut");
    CIterator i3 = x.listByAddr();
    assert (!i3.atEnd()
            && i3.city() == "Liberec"
            && i3.addr() == "Evropska"
            && i3.region() == "Librec"
            && i3.id() == 4552
            && i3.owner() == "Cvut");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Plzen"
            && i3.addr() == "Evropska"
            && i3.region() == "Plzen mesto"
            && i3.id() == 78901
            && i3.owner() == "Anton Hrabis");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Prague"
            && i3.addr() == "Evropska"
            && i3.region() == "Vokovice"
            && i3.id() == 12345
            && i3.owner() == "");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Prague"
            && i3.addr() == "Technicka"
            && i3.region() == "Dejvice"
            && i3.id() == 9873
            && i3.owner() == "CVUT");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Prague"
            && i3.addr() == "Thakurova"
            && i3.region() == "Dejvice"
            && i3.id() == 12345
            && i3.owner() == "CVUT");
    i3.next();
    assert (i3.atEnd());

    assert (x.count("cvut") == 3);
    CIterator i4 = x.listByOwner("cVuT");
    assert (!i4.atEnd()
            && i4.city() == "Prague"
            && i4.addr() == "Thakurova"
            && i4.region() == "Dejvice"
            && i4.id() == 12345
            && i4.owner() == "CVUT");
    i4.next();
    assert (!i4.atEnd()
            && i4.city() == "Prague"
            && i4.addr() == "Technicka"
            && i4.region() == "Dejvice"
            && i4.id() == 9873
            && i4.owner() == "CVUT");
    i4.next();
    assert (!i4.atEnd()
            && i4.city() == "Liberec"
            && i4.addr() == "Evropska"
            && i4.region() == "Librec"
            && i4.id() == 4552
            && i4.owner() == "Cvut");
    i4.next();
    assert (i4.atEnd());

    assert (x.newOwner("Plzen mesto", 78901, "CVut"));
    assert (x.count("CVUT") == 4);
    CIterator i5 = x.listByOwner("CVUT");
    assert (!i5.atEnd()
            && i5.city() == "Prague"
            && i5.addr() == "Thakurova"
            && i5.region() == "Dejvice"
            && i5.id() == 12345
            && i5.owner() == "CVUT");
    i5.next();
    assert (!i5.atEnd()
            && i5.city() == "Prague"
            && i5.addr() == "Technicka"
            && i5.region() == "Dejvice"
            && i5.id() == 9873
            && i5.owner() == "CVUT");
    i5.next();
    assert (!i5.atEnd()
            && i5.city() == "Liberec"
            && i5.addr() == "Evropska"
            && i5.region() == "Librec"
            && i5.id() == 4552
            && i5.owner() == "Cvut");
    i5.next();
    assert (!i5.atEnd()
            && i5.city() == "Plzen"
            && i5.addr() == "Evropska"
            && i5.region() == "Plzen mesto"
            && i5.id() == 78901
            && i5.owner() == "CVut");
    i5.next();
    assert (i5.atEnd());

    assert (x.del("Liberec", "Evropska"));
    assert (x.del("Plzen mesto", 78901));
    assert (x.count("cvut") == 2);
    CIterator i6 = x.listByOwner("cVuT");
    assert (!i6.atEnd()
            && i6.city() == "Prague"
            && i6.addr() == "Thakurova"
            && i6.region() == "Dejvice"
            && i6.id() == 12345
            && i6.owner() == "CVUT");
    i6.next();
    assert (!i6.atEnd()
            && i6.city() == "Prague"
            && i6.addr() == "Technicka"
            && i6.region() == "Dejvice"
            && i6.id() == 9873
            && i6.owner() == "CVUT");
    i6.next();
    assert (i6.atEnd());

    assert (x.add("Liberec", "Evropska", "Librec", 4552));
}

static void test1() {
    CLandRegister x;
    std::string owner;

    assert (x.add("Prague", "Thakurova", "Dejvice", 12345));
    assert (x.add("Prague", "Evropska", "Vokovice", 12345));
    assert (x.add("Prague", "Technicka", "Dejvice", 9873));
    assert (!x.add("Prague", "Technicka", "Hradcany", 7344));
    assert (!x.add("Brno", "Bozetechova", "Dejvice", 9873));
    assert (!x.getOwner("Prague", "THAKUROVA", owner));
    assert (!x.getOwner("Hradcany", 7343, owner));
    CIterator i0 = x.listByAddr();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Evropska"
            && i0.region() == "Vokovice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Technicka"
            && i0.region() == "Dejvice"
            && i0.id() == 9873
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Thakurova"
            && i0.region() == "Dejvice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (i0.atEnd());

    assert (x.newOwner("Prague", "Thakurova", "CVUT"));
    assert (!x.newOwner("Prague", "technicka", "CVUT"));
    assert (!x.newOwner("prague", "Technicka", "CVUT"));
    assert (!x.newOwner("dejvice", 9873, "CVUT"));
    assert (!x.newOwner("Dejvice", 9973, "CVUT"));

    assert (!x.newOwner("Dejvice", 12345, "CVUT"));
    assert (x.count("CVUT") == 1);
    CIterator i1 = x.listByOwner("CVUT");

    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Thakurova"
            && i1.region() == "Dejvice"
            && i1.id() == 12345
            && i1.owner() == "CVUT");
    i1.next();
    assert (i1.atEnd());

    assert (!x.del("Brno", "Technicka"));
    assert (!x.del("Karlin", 9873));
    assert (x.del("Prague", "Technicka"));
    assert (!x.del("Prague", "Technicka"));
    assert (!x.del("Dejvice", 9873));
}

int main(void) {
    test0();
     test1();
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
