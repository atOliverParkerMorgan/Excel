#include "iostream"

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
    void setOwner(const std::string& newOwner);
};