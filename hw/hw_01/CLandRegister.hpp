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
    static bool cmpByRegion(const Land *p1, const Land *p2);

    static bool cmpByCity(const Land *p1, const Land *p2);

    static bool cmpByOwner(const std::pair<std::string, std::vector<Land*>> &l1,
                           const std::pair<std::string, std::vector<Land*>> &l2);

    static std::string stringToLower(const std::string &in);

    bool isInProperty(const std::string &city,
                      const std::string &addr);

    bool isInProperty(const std::string &region,
                      unsigned int id);

    std::vector<Land*> byCityAddr;
    std::vector<Land*> byRegionId;
    std::vector<std::pair<std::string, std::vector<Land*>>> byOwner;

    void addNewOwner(const std::string &owner, Land *land);
    void removeOwner(const std::string &owner, Land *land);
};