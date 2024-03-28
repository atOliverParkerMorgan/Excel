#pragma once
#include <cstddef>
#include <ostream>

class String {
    struct ShareStr{
        char* data;
        size_t ref_count;

        ShareStr();
        ShareStr(const char * s);
        ~ShareStr();

        void incRef();
        void decRef();
    };
    ShareStr *str;
public:
    String();
    String(const char *s);
    String(const String &other);
    ~String();

    friend std::ostream & operator<<(std::ostream  &os, const String & s){
        if(!s.str) {return os;}
        return os << s.str;
    }
    String &operator=(const String & other);
    String operator+(const String& other)const;
    String &operator+=(const String &other);
    std::strong_ordering operator <=>(const String & other) const;
    bool operator==(const String &other) const;
};

