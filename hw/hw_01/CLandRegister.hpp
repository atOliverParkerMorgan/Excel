#include <iostream>
#include "CIterator.hpp"
#include <vector>

class CLandRegister {
public:
    struct Property {
    public:
        Property(std::string city,
                 std::string addr,
                 std::string region,
                 unsigned int id);

        std::string city;
        std::string addr;
        std::string region;
        unsigned int id;
    };

    static bool cmpByRegion(const Property &p1, const Property &p2);

    static bool cmpByCity(const CLandRegister::Property &p1, const CLandRegister::Property &p2);

    CLandRegister() = default;

    ~CLandRegister() = default;

    void print(bool byCity = true);

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
    bool isInProperty(const std::string &city,
                      const std::string &addr);

    bool isInProperty(const std::string &region,
                      unsigned int id);

    std::vector<Property> byCityAddr;
    std::vector<Property> byRegionId;
    std::vector<Property> propertyByAquire;

};