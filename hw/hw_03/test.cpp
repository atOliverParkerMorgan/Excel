#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#endif /* __PROGTEST__ */

class CPatchStr {
    size_t size;
    char *data;

public:


    CPatchStr();

    CPatchStr(const char *str);

    // copy constructor
    CPatchStr(const CPatchStr &other);

    // destructor
    ~CPatchStr();

    // operator =
    CPatchStr &operator=(const CPatchStr &rhs);

    CPatchStr subStr(size_t from,
                     size_t len) const;

    CPatchStr &append(const CPatchStr &src);

    CPatchStr &insert(size_t pos,
                      const CPatchStr &src);

    CPatchStr &remove(size_t from,
                      size_t len);

    char *toStr() const;

};


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
    if(pos == size){
        append(src);
        return *this;
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
    for (size_t i = 0; i < size; ++i) {
        if (i < from || i >= from + len) {
            newStr[cnt++] = data[i];
        }
    }
    size = size - len;

    newStr[size] = '\0';
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





#ifndef __PROGTEST__
bool stringMatch ( char       * str,
                   const char * expected )
{
    bool res = std::strcmp ( str, expected ) == 0;
    delete [] str;
    return res;
}

int main ()
{
    char tmpStr[100];

    CPatchStr a ( "test" );
    assert ( stringMatch ( a . toStr (), "test" ) );
    std::strncpy ( tmpStr, " da", sizeof ( tmpStr ) - 1 );
    a . append ( tmpStr );
    assert ( stringMatch ( a . toStr (), "test da" ) );
    std::strncpy ( tmpStr, "ta", sizeof ( tmpStr ) - 1 );
    a . append ( tmpStr );
    assert ( stringMatch ( a . toStr (), "test data" ) );
    std::strncpy ( tmpStr, "foo text", sizeof ( tmpStr ) - 1 );
    CPatchStr b ( tmpStr );
    assert ( stringMatch ( b . toStr (), "foo text" ) );
    CPatchStr c ( a );
    assert ( stringMatch ( c . toStr (), "test data" ) );
    CPatchStr d ( a . subStr ( 3, 5 ) );
    assert ( stringMatch ( d . toStr (), "t dat" ) );
    d . append ( b );
    assert ( stringMatch ( d . toStr (), "t datfoo text" ) );
    d . append ( b . subStr ( 3, 4 ) );
    assert ( stringMatch ( d . toStr (), "t datfoo text tex" ) );
    c . append ( d );
    assert ( stringMatch ( c . toStr (), "test datat datfoo text tex" ) );
    c . append ( c );
    assert ( stringMatch ( c . toStr (), "test datat datfoo text textest datat datfoo text tex" ) );
    d . insert ( 2, c . subStr ( 6, 9 ) );
    assert ( stringMatch ( d . toStr (), "t atat datfdatfoo text tex" ) );
    b = "abcdefgh";
    assert ( stringMatch ( b . toStr (), "abcdefgh" ) );
    assert ( stringMatch ( d . toStr (), "t atat datfdatfoo text tex" ) );
    assert ( stringMatch ( d . subStr ( 4, 8 ) . toStr (), "at datfd" ) );
    assert ( stringMatch ( b . subStr ( 2, 6 ) . toStr (), "cdefgh" ) );
    try
    {
        b . subStr ( 2, 7 ) . toStr ();
        assert ( "Exception not thrown" == nullptr );
    }
    catch ( const std::out_of_range & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown" == nullptr );
    }
    a . remove ( 3, 5 );
    assert ( stringMatch ( a . toStr (), "tesa" ) );
    a.insert(4,b);
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
