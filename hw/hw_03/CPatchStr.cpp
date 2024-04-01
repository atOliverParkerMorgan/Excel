//
// Created by olixandr on 28.3.24.
//

#include "CPatchStr.h"

CPatchStr::CPatchStr() : size(0), cap(0), data(nullptr) {}

CPatchStr::CPatchStr(const char *string) : size(0), cap(0), data(nullptr) {
    auto *pSharedPtr = new SharedPtr(string);
    push(pSharedPtr);

}

CPatchStr::CPatchStr(const CPatchStr &other) : cap(other.cap), size(other.size), total_len(other.total_len) {
    data = new SharedPtr *[cap];
    for (int i = 0; i < size; ++i) {
        data[i] = other.data[i];
        data[i]->incRef();
    }
}

CPatchStr::~CPatchStr() {
    for (int i = 0; i < size; ++i) {
        data[i]->decRef();
    }
    delete[]data;
}


CPatchStr &CPatchStr::operator=(const CPatchStr &rhs) {
    if (this != &rhs) {
        for (int i = 0; i < size; ++i) {
            data[i]->decRef();
        }

        delete[] data;
        size = rhs.size;
        cap = rhs.cap;
        total_len = rhs.total_len;

        data = new SharedPtr *[cap];
        for (int i = 0; i < size; ++i) {
            data[i] = rhs.data[i];
            data[i]->incRef();
        }
    }
    return *this;
}


CPatchStr CPatchStr::subStr(size_t from, size_t len) const {
    size_t cnt = 0;
    auto *sub = new CPatchStr();
    for (int i = 0; i < size; ++i) {
        cnt += data[i]->len - data[i]->ofs;
        if (cnt > from) {
            sub->push(data[i]);
        }
    }
}

CPatchStr &CPatchStr::append(const CPatchStr &src) {
    for (int i = 0; i < src.size; ++i) {
        push(src.data[i]);
        src.data[i]->incRef();
    }
    return *this;
}


CPatchStr &CPatchStr::insert(size_t pos, const CPatchStr &src) {


    if (pos > total_len) {
        throw std::out_of_range("Insert out of range");
    }

    return *this;
}

CPatchStr &CPatchStr::remove(size_t from, size_t length) {
    return *this;
}

char *CPatchStr::toStr() const {
    char *out = new char[total_len + 1];
    size_t current_index = 0;
    for (int i = 0; i < size; ++i) {
        strncpy(out + current_index, data[i]->data + data[i]->ofs, data[i]->len);
        current_index += data[i]->len - data[i]->ofs;
    }
    out[total_len] = 0;
    return out;
}

void CPatchStr::push(SharedPtr *sharedPtr) {
    if (cap >= size) {
        cap = 2 * cap + 8;
        auto **newData = new SharedPtr *[cap];
        memcpy(newData, data, size * sizeof(SharedPtr *));
        delete[] data;
        data = newData;
    }
    total_len += sharedPtr->len - sharedPtr->ofs;
    data[size++] = sharedPtr;
}

void CPatchStr::SharedPtr::incRef() {
    ++ref_count;
}

void CPatchStr::SharedPtr::decRef() {
    if (--ref_count == 0) delete this;
}

CPatchStr::SharedPtr::SharedPtr(const char *s) : ofs(0), len(strlen(s)), data(new char[strlen(s) + 1]), ref_count(1) {
    strcpy(data, s);
}

CPatchStr::SharedPtr::~SharedPtr() {
    delete[] data;
}

CPatchStr::SharedPtr::SharedPtr() : ofs(0), len(0), ref_count(0), data(nullptr) {}

