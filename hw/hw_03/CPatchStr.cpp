//
// Created by olixandr on 28.3.24.
//

#include "CPatchStr.h"

CPatchStr::CPatchStr() : size(0), cap(0), data(nullptr) {}

CPatchStr::CPatchStr(const char *string) : size(0), cap(0), data(nullptr) {
    auto *str = new Str(string);
    push(str);

}

CPatchStr::CPatchStr(const CPatchStr &other) : cap(other.cap), size(other.size), total_len(other.total_len) {
    data = new Str *[cap];
    for (int i = 0; i < size; ++i) {
        data[i] = other.data[i];
        data[i]->sharedPtr->incRef();
    }
}

CPatchStr::~CPatchStr() {
//    for (int i = 0; i < size; ++i) {
//        delete data[i];
//    }

}


CPatchStr &CPatchStr::operator=(const CPatchStr &rhs) {
    if (this != &rhs) {
        for (int i = 0; i < size; ++i) {
            data[i]->sharedPtr->decRef();
        }

        delete[] data;
        size = rhs.size;
        cap = rhs.cap;
        total_len = rhs.total_len;

        data = new Str *[cap];
        for (int i = 0; i < size; ++i) {
            data[i] = rhs.data[i];
            data[i]->sharedPtr->incRef();
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
        src.data[i]->sharedPtr->incRef();
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
        strncpy(out + current_index, data[i]->sharedPtr->data + data[i]->ofs, data[i]->len);
        current_index += data[i]->len - data[i]->ofs;
    }
    out[total_len] = 0;
    return out;
}

void CPatchStr::push(Str str) {
    if (cap >= size) {
        cap = 2 * cap + 8;
        auto **newData = new Str *[cap];
        memcpy(newData, data, size * sizeof(Str *));
        delete[] data;
        data = newData;
    }
    total_len += str.len - str.ofs;
    data[size++] = &str;
}

void CPatchStr::SharedPtr::incRef() {
    ++ref_count;
}

void CPatchStr::SharedPtr::decRef() {
    if (--ref_count == 0) delete this;
}

CPatchStr::SharedPtr::SharedPtr(const char *s) : data(new char[strlen(s) + 1]), ref_count(1) {
    strcpy(data, s);
}

CPatchStr::SharedPtr::~SharedPtr() {
    delete[] data;
}

CPatchStr::SharedPtr::SharedPtr() : ref_count(0), data(nullptr) {}

CPatchStr::Str::Str() : ofs(0), len(0), sharedPtr(nullptr) {}

CPatchStr::Str::Str(const char *str) : ofs(0), len(strlen(str)), sharedPtr(new SharedPtr(str)) {}

CPatchStr::Str::~Str() {
    sharedPtr->decRef();
}

CPatchStr::Str::Str(const CPatchStr::Str &other): ofs(other.ofs), len(other.len), sharedPtr(other.sharedPtr){
    other.sharedPtr->incRef();
}
