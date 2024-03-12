#include <iostream>
#include "CIterator.hpp"
#include <vector>


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

    std::vector<CLand*> byCityAddr;
    std::vector<CLand*> byRegionId;
    std::vector<COwner*> byOwner;

    bool addNewOwner(const std::string &owner, CLand *land);
    void removeOwner(const std::string &owner, CLand *land);
};