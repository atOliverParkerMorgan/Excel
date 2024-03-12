#include "CIterator.hpp"

#include <utility>

bool CIterator::atEnd() const {
    return currentIndex == lands.size();
}

void CIterator::next() {
    if (!atEnd()) {
        currentIndex++;
    }
}

std::string CIterator::city() const {
    if(!atEnd()){
        return lands[currentIndex]->city;
    }
    return "";
}

std::string CIterator::addr() const {
    if(!atEnd()){
        return lands[currentIndex]->addr;
    }
    return "";
}

unsigned CIterator::id() const {
    if(!atEnd()){
        return lands[currentIndex]->id;
    }
    return 0;
}

std::string CIterator::owner() const {
    if(!atEnd()){
        return lands[currentIndex]->owner;
    }
    return "";
}

CIterator::CIterator(std::vector<CLand *> in_land) {
    this->lands = std::move(in_land);
    currentIndex = 0;
}

std::string CIterator::region() const {
    if(!atEnd()){
        return lands[currentIndex]->region;
    }
    return "";
}


