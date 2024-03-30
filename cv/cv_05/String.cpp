#include "String.h"
#pragma once
#include <cstddef>
#include <ostream>
#include <cstring>

String::ShareStr::ShareStr(): data(nullptr), ref_count(0){}

String::ShareStr::ShareStr(const char* s): data(new char[strlen(s)+1]), ref_count(1){
    strcpy(data,s);
}

void String::ShareStr::incRef() {
    ref_count++;
}

void String::ShareStr::decRef() {
    if(--ref_count == 0)
        delete this;
}

String::ShareStr::~ShareStr() {
    delete[] data;
}

String::String(): str(nullptr) {}

String::String(const char *s) : str(new ShareStr(s)){}

String::String(const String &other) {
    if(str)str->incRef();
}

String::~String() {
    if(str)
        str->decRef();
}

String &String::operator=(const String &other) {
    if(this != &other){
        if(str)
            str->decRef();
        str = other.str;
        if(str)
            str->incRef();
    }
    return *this;
}

String String::operator+(const String &other) const {
    char *buff = new char[strlen(str->data) + strlen(other.str->data)];
    strcpy(buff, str->data);
    strcpy(buff + strlen(str->data), other.str->data);

    String result(buff);
    delete[] buff;
    return result;
}

String &String::operator+=(const String &other) {
    return *this = *this + other;
}

std::strong_ordering String::operator<=>(const String &other) const {
    return strcmp(str->data, other.str->data) <=> 0;
}