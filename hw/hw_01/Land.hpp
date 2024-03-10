#include "iostream"

struct Land {

    Land(std::string city,
         std::string addr,
         std::string region,
         unsigned int id);

    std::string city;
    std::string addr;
    std::string region;
    unsigned int id;
    std::string owner;

    void setOwner(const std::string& newOwner);
};