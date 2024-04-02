//
// Created by olixandr on 28.3.24.
//


#include "CPatchStr.h"

CPatchStr::CPatchStr() : size(0), data(nullptr) {}

CPatchStr::CPatchStr(const char *str) : size(strlen(str)), data(new char[size + 1]) {
    strcpy(data, str);
}

CPatchStr::CPatchStr(const CPatchStr &other) : size(other.size), data(new char[size + 1]) {

    strcpy(data, other.data);
}

CPatchStr::~CPatchStr() {
    delete[] data;
}

CPatchStr &CPatchStr::operator=(const CPatchStr &rhs) {
    if (this != &rhs) {
        delete[] data;
        size = rhs.size;
        data = new char[size + 1];
        strcpy(data, rhs.data);

    }
    return *this;
}

CPatchStr CPatchStr::subStr(size_t from, size_t len) const {
    if (len + from > size) {
        throw std::out_of_range("42");
    }
    CPatchStr newCPatchStr;
    newCPatchStr.data = new char[len + 1];
    newCPatchStr.size = len;
    strncpy(newCPatchStr.data, data + from, len);
    newCPatchStr.data[len] = 0;

    return newCPatchStr;
}

CPatchStr &CPatchStr::append(const CPatchStr &src) {
    char *newData = new char[size + src.size + 1];
    strcpy(newData, data);
    strcat(newData, src.data);
    delete[] data;
    data = newData;
    size += src.size;
    return *this;
}

CPatchStr &CPatchStr::insert(size_t pos, const CPatchStr &src) {
    if (pos > size) {
        throw std::out_of_range("42");
    }
    char *newStr = new char[size + src.size + 1];
    size_t cnt = 0;
    for (size_t i = 0; i < size; ++i) {
        if (i == pos) {
            for (size_t j = 0; j < src.size; j++) {
                newStr[j + cnt] = src.data[j];
            }
            cnt += src.size;
        }
        newStr[cnt++] = data[i];

    }
    newStr[size + src.size] = 0;

    delete[]data;
    data = newStr;
    size += src.size;
    return *this;
}

CPatchStr &CPatchStr::remove(size_t from, size_t len) {
    if (len + from > size) {
        throw std::out_of_range("42");
    }
    char *newStr = new char[size - len + 1];
    size_t cnt = 0;
    for (int i = 0; i < size; ++i) {
        if (i < from || i >= from + len) {
            newStr[cnt++] = data[i];
        }
    }
    newStr[size - len] = 0;
    delete[] data;
    data = newStr;

    return *this;
}

char *CPatchStr::toStr() const {
    char *out = new char[size + 1];
    strcpy(out, data);
    out[size] = '\0';
    return out;
}

