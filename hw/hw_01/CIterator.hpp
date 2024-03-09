#include "iostream"

class CIterator {
public:
    bool atEnd() const;

    void next();

    std::string city() const;

    std::string addr() const;

    std::string region() const;

    unsigned id() const;

    std::string owner() const;

private:
    // todo
};
