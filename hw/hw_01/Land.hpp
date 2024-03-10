#include "iostream"

struct Land {

    Land(std::string city,
         std::string addr,
         std::string region,
         unsigned int id,
         int acquireOrder);

    std::string city;
    std::string addr;
    std::string region;
    unsigned int id;
    std::string owner;
    std::string ownerLower;
    unsigned int acquireOrder;

    void setOwner(const std::string& newOwner);
};