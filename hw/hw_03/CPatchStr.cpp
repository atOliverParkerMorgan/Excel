//
// Created by olixandr on 28.3.24.
//


#include "CPatchStr.h"

CPatchStr::SharedPtr::SharedPtr() : ofs(0), len(0), data(nullptr) {}

CPatchStr::SharedPtr::SharedPtr(const char *str) : ofs(0), len(std::strlen(str)), data(new char[len + 1]) {
    std::strcpy(data.get(), str);
}

CPatchStr::SharedPtr::~SharedPtr() {}

CPatchStr::CPatchStr() : size(0), cap(0), data(nullptr) {}

CPatchStr::CPatchStr(const char *str) : size(0), cap(0), data(nullptr) {
    auto sharedPtr = new SharedPtr(str);
    push(sharedPtr);
}

CPatchStr::CPatchStr(const CPatchStr &other) : cap(other.cap), size(other.size), total_len(other.total_len) {
    data = new SharedPtr[cap];
    for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

CPatchStr::~CPatchStr() {
    delete[] data;
}

CPatchStr &CPatchStr::operator=(const CPatchStr &rhs) {
    if (this != &rhs) {
        delete[] data;
        size = rhs.size;
        cap = rhs.cap;
        total_len = rhs.total_len;
        data = new SharedPtr[cap];
        for (size_t i = 0; i < size; ++i) {
            data[i] = rhs.data[i];
        }
    }
    return *this;
}

CPatchStr CPatchStr::subStr(size_t from, size_t len) const {
    // Implement subStr functionality
}

CPatchStr &CPatchStr::append(const CPatchStr &src) {
    // Implement append functionality
}

CPatchStr &CPatchStr::insert(size_t pos, const CPatchStr &src) {
    // Implement insert functionality
}

CPatchStr &CPatchStr::remove(size_t from, size_t length) {
    // Implement remove functionality
}

char* CPatchStr::toStr() const {
    char* out = new char[total_len + 1];
    size_t current_index = 0;
    for (size_t i = 0; i < size; ++i) {
        std::strncpy(out + current_index, data[i].data.get() + data[i].ofs, data[i].len);
        current_index += data[i].len - data[i].ofs;
    }
    out[total_len] = '\0'; // Null-terminate the string
    return out;
}
void CPatchStr::push(SharedPtr *sharedPtr) {
    if (cap >= size) {
        cap = 2 * cap + 8;
        auto *newData = new SharedPtr[cap];
        std::copy(data, data + size, newData);
        delete[] data;
        data = newData;
    }
    total_len += sharedPtr->len - sharedPtr->ofs;
    data[size++] = *sharedPtr;

}
