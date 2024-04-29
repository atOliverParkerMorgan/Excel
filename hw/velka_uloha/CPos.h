#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>
#include "expression.h"

using namespace std::literals;
using CValue = std::variant<std::monostate, double, std::string>;

constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;
#endif /* __PROGTEST__ */


class CPos {
public:
    CPos(std::string_view str) {
        size_t index = 0;
        m_Column = 0;

        for (; !isdigit(str[index]); ++index) {
            bool isUpper = std::isupper(str[index]);
            if ((!isUpper && !std::islower(str[index])) || index + 1 >= str.size()) {
                throw std::invalid_argument("CPos argument is not valid");
            }
            const char baseChar = isUpper ? 'A' : 'a';
            m_Column *= SYSTEM_VALUE;
            m_Column += (str[index] - baseChar + 1);
        }

        if (index == 0) {
            throw std::invalid_argument("CPos argument is not valid");
        }

        m_Row = 0;
        for (; index < str.length(); ++index) {
            if (!isdigit(str[index])) {
                throw std::invalid_argument("CPos argument is not valid");
            }
            m_Row *= 10;
            m_Row += str[index] - '0';
        }
    }


    size_t getRow() const {
        return m_Row;
    }

    size_t getColumn() const {
        return m_Column;
    }

    const static size_t SYSTEM_VALUE = 26;

private:
    size_t m_Row;
    size_t m_Column;
};