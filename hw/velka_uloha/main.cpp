#ifndef __PROGTEST__
// g++ -std=c++20 -Wall -pedantic -g -o excel test.cpp AST.cpp ASTBuilder.cpp -fsanitize=address -L./x86_64-linux-gnu -lexpression_parser
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <array>
#include <utility>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <unordered_set>
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

using namespace std::literals;

constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;
#endif /* __PROGTEST__ */

#include "ASTBuilder.h"


class CSpreadsheet {
public:
    static unsigned capabilities() {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED;
    }

    CSpreadsheet() {};

    bool load(std::istream &is) {
        bool isAtCPosFirst = true;
        bool isAtCPosSecond = false;

        std::string baseString;
        std::string currentString;

        is >> baseString;
        std::pair<size_t, size_t> currentPair;

        for (int i = 0; i < baseString.length(); ++i) {
            const char c = baseString[i];
            const size_t nextPosOfDelimiter = baseString.find_first_of(',', i);
            if (nextPosOfDelimiter == i && isAtCPosFirst) {
                currentPair.first = std::stoul(currentString);
                isAtCPosFirst = false;
                isAtCPosSecond = true;
            } else if (c == ASTNode::DELIMITER && isAtCPosSecond) {
                currentPair.second = std::stoul(currentString);
                isAtCPosSecond = false;
            } else {
                std::s
                if (c == ASTNode::DELIMITER) {

                } else if (isupper(c) || islower(c)) {
                    m_Builder.valReference(baseString.substr(i, nextPosOfDelimiter - i));
                }else if(isdigit(c)){
                    m_Builder.valNumber(std::stod(baseString.substr(i, nextPosOfDelimiter - i)));
                } else if(c=='+'){
                   m_Builder.opAdd();
                } else if(c=='*'){
                    m_Builder.opAdd();
                } else if(c=='-'){
                    m_Builder.opAdd();
                } else if(c=='/'){
                    m_Builder.opAdd();
                } else if(c=='^'){
                    m_Builder.opAdd();
                } else if(c=='='){
                    m_Builder.opAdd();
                } else if(c=='<'){
                    m_Builder.opAdd();
                } else if(c=='>'){
                    m_Builder.opAdd();
                } else if(c=='='){
                    m_Builder.opAdd();
                }
            }
        }

    };

    bool save(std::ostream &os) const {
        for (auto &[key, node]: m_SheetNodeData) {
            os << key.first << ASTNode::DELIMITER << key.second << ASTNode::DELIMITER << *node;
        }
        return true;
    };

    bool setCell(CPos pos, const std::string &contents) {
        parseExpression(contents, m_Builder);

        std::pair<size_t, size_t> key = {pos.getColumn(), pos.getRow()};
        auto it = m_SheetNodeData.find(key);

        if (it == m_SheetNodeData.end()) {
            m_SheetNodeData.insert({key, m_Builder.getRoot()});
        } else {
            m_SheetNodeData[key] = m_Builder.getRoot();
        }

        return true;
    }

    CValue getValue(CPos pos) {
        std::pair<size_t, size_t> key = {pos.getColumn(), pos.getRow()};

        auto it = m_SheetNodeData.find(key);

        if (it != m_SheetNodeData.end()) {
            return m_SheetNodeData.at(key)->eval(m_SheetNodeData);
        }
        return std::monostate();
    }

    void copyRect(CPos dst, CPos src, int w = 1, int h = 1) {
        std::unordered_map<std::pair<size_t, size_t>, EASTNode> tmpCopy;
        for (size_t y = 0; y < (size_t) h; y++) {
            for (size_t x = 0; x < (size_t) w; x++) {
                std::pair<size_t, size_t> keySrc = {src.getColumn() + x, src.getRow() + y};
                std::pair<size_t, size_t> keyDest = {dst.getColumn() + x, dst.getRow() + y};

                auto it = m_SheetNodeData.find(keySrc);
                if (keyDest.first >= src.getColumn() && keyDest.first < w + src.getColumn() &&
                    keyDest.second >= src.getRow() && keyDest.second < h + src.getRow() &&
                    it != m_SheetNodeData.end()) {

                    tmpCopy[keyDest] = m_SheetNodeData[keySrc]->clone();
                } else if (it != m_SheetNodeData.end()) {
                    EASTNode node = m_SheetNodeData[keySrc]->clone();
                    node->moveRelative(dst.getColumn() - src.getColumn(),
                                       dst.getRow() - src.getRow());
                    m_SheetNodeData[keyDest] = node;
                }
            }
        }
        for (auto &[keyDest, node]: tmpCopy) {

            node->moveRelative(dst.getColumn() - src.getColumn(),
                               dst.getRow() - src.getRow());

            m_SheetNodeData[keyDest] = node;
        }
    };

private:
    ASTBuilder m_Builder;
    std::unordered_map<std::pair<size_t, size_t>, EASTNode> m_SheetNodeData;
};

#ifndef __PROGTEST__

bool valueMatch(const CValue &r,
                const CValue &s) {
    if (r.index() != s.index())
        return false;
    if (r.index() == 0)
        return true;
    if (r.index() == 2)
        return std::get<std::string>(r) == std::get<std::string>(s);
    if (std::isnan(std::get<double>(r)) && std::isnan(std::get<double>(s)))
        return true;
    if (std::isinf(std::get<double>(r)) && std::isinf(std::get<double>(s)))
        return (std::get<double>(r) < 0 && std::get<double>(s) < 0)
               || (std::get<double>(r) > 0 && std::get<double>(s) > 0);
    return fabs(std::get<double>(r) - std::get<double>(s)) <= 1e8 * DBL_EPSILON * fabs(std::get<double>(r));
}

int main() {
    CSpreadsheet x0, x1;
    std::ostringstream oss;
    std::istringstream iss;
    std::string data;

    assert (x0.setCell(CPos("A1"), "=10"));
    assert (x0.setCell(CPos("A2"), "20.5"));
    assert (x0.setCell(CPos("A3"), "3e1"));
    assert (x0.setCell(CPos("A4"), "=40"));
    assert (x0.setCell(CPos("A5"), "=5e+1"));
    assert (x0.setCell(CPos("A6"), "raw text with any characters, including a quote \" or a newline\n"));
    assert (x0.setCell(CPos("A7"),
                       "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\""));
    assert (valueMatch(x0.getValue(CPos("A1")), CValue(10.0)));
    assert (valueMatch(x0.getValue(CPos("A2")), CValue(20.5)));
    assert (valueMatch(x0.getValue(CPos("A3")), CValue(30.0)));
    assert (valueMatch(x0.getValue(CPos("A4")), CValue(40.0)));
    assert (valueMatch(x0.getValue(CPos("A5")), CValue(50.0)));
    assert (valueMatch(x0.getValue(CPos("A6")),
                       CValue("raw text with any characters, including a quote \" or a newline\n")));
    assert (valueMatch(x0.getValue(CPos("A7")),
                       CValue("quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++.")));
    assert (valueMatch(x0.getValue(CPos("A8")), CValue()));
    assert (valueMatch(x0.getValue(CPos("AAAA9999")), CValue()));
    assert (x0.setCell(CPos("B1"), "=A1+A2*A3"));
    assert (x0.setCell(CPos("B2"), "= -A1 ^ 2 - A2 / 2   "));
    assert (x0.setCell(CPos("B3"), "= 2 ^ $A$1"));
    assert (x0.setCell(CPos("B4"), "=($A1+A$2)^2"));
    assert (x0.setCell(CPos("B5"), "=B1+B2+B3+B4"));
    assert (x0.setCell(CPos("B6"), "=B1+B2+B3+B4+B5"));

    assert (valueMatch(x0.getValue(CPos("B1")), CValue(625.0)));
    assert (valueMatch(x0.getValue(CPos("B2")), CValue(-110.25)));
    assert (valueMatch(x0.getValue(CPos("B3")), CValue(1024.0)));
    assert (valueMatch(x0.getValue(CPos("B4")), CValue(930.25)));
    assert (valueMatch(x0.getValue(CPos("B5")), CValue(2469.0)));
    assert (valueMatch(x0.getValue(CPos("B6")), CValue(4938.0)));
    assert (x0.setCell(CPos("A1"), "12"));
    assert (valueMatch(x0.getValue(CPos("B1")), CValue(627.0)));
    assert (valueMatch(x0.getValue(CPos("B2")), CValue(-154.25)));
    assert (valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x0.getValue(CPos("B4")), CValue(1056.25)));
    assert (valueMatch(x0.getValue(CPos("B5")), CValue(5625.0)));
    assert (valueMatch(x0.getValue(CPos("B6")), CValue(11250.0)));
    x1 = x0;
    assert (x0.setCell(CPos("A2"), "100"));
    assert (x1.setCell(CPos("A2"), "=A3+A5+A4"));
    assert (valueMatch(x0.getValue(CPos("B1")), CValue(3012.0)));
    assert (valueMatch(x0.getValue(CPos("B2")), CValue(-194.0)));
    assert (valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x0.getValue(CPos("B4")), CValue(12544.0)));
    assert (valueMatch(x0.getValue(CPos("B5")), CValue(19458.0)));
    assert (valueMatch(x0.getValue(CPos("B6")), CValue(38916.0)));
    assert (valueMatch(x1.getValue(CPos("B1")), CValue(3612.0)));
    assert (valueMatch(x1.getValue(CPos("B2")), CValue(-204.0)));
    assert (valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x1.getValue(CPos("B4")), CValue(17424.0)));
    assert (valueMatch(x1.getValue(CPos("B5")), CValue(24928.0)));
    assert (valueMatch(x1.getValue(CPos("B6")), CValue(49856.0)));
    oss.clear();
    oss.str("");
    assert (x0.save(oss));
    data = oss.str();
    std::cout << data << std::endl;
    iss.clear();
    iss.str(data);
    assert (x1.load(iss));
    assert (valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
    assert (valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
    assert (valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
    assert (valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
    assert (valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
    assert (x0.setCell(CPos("A3"), "4e1"));
    assert (valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
    assert (valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
    assert (valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
    assert (valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
    assert (valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
    oss.clear();
    oss.str("");
    assert (x0.save(oss));
    data = oss.str();
    for (size_t i = 0; i < std::min<size_t>(data.length(), 10); i++)
        data[i] ^= 0x5a;
    iss.clear();
    iss.str(data);
    assert (!x1.load(iss));
    assert (x0.setCell(CPos("D0"), "10"));
    assert (x0.setCell(CPos("D1"), "20"));
    assert (x0.setCell(CPos("D2"), "30"));
    assert (x0.setCell(CPos("D3"), "40"));
    assert (x0.setCell(CPos("D4"), "50"));
    assert (x0.setCell(CPos("E0"), "60"));
    assert (x0.setCell(CPos("E1"), "70"));
    assert (x0.setCell(CPos("E2"), "80"));
    assert (x0.setCell(CPos("E3"), "90"));
    assert (x0.setCell(CPos("E4"), "100"));
    assert (x0.setCell(CPos("F10"), "=D0+5"));
    assert (x0.setCell(CPos("F11"), "=$D0+5"));
    assert (x0.setCell(CPos("F12"), "=D$0+5"));
    assert (x0.setCell(CPos("F13"), "=$D$0+5"));
    x0.copyRect(CPos("G11"), CPos("F10"), 1, 4);

    assert (valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F14")), CValue()));
    assert (valueMatch(x0.getValue(CPos("G10")), CValue()));
    assert (valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
    assert (valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
    assert (valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
    assert (valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));


    x0.copyRect(CPos("G11"), CPos("F10"), 2, 4);
    assert (valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F14")), CValue()));
    assert (valueMatch(x0.getValue(CPos("G10")), CValue()));
    assert (valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
    assert (valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
    assert (valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
    assert (valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("H10")), CValue()));
    assert (valueMatch(x0.getValue(CPos("H11")), CValue()));
    CValue h12 = x0.getValue(CPos("H12"));

    assert (valueMatch(x0.getValue(CPos("H12")), CValue()));
    assert (valueMatch(x0.getValue(CPos("H13")), CValue(35.0)));
    assert (valueMatch(x0.getValue(CPos("H14")), CValue()));
    assert (x0.setCell(CPos("F0"), "-27"));
    assert (valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));
    x0.copyRect(CPos("H12"), CPos("H13"), 1, 2);
    assert (valueMatch(x0.getValue(CPos("H12")), CValue(25.0)));
    assert (valueMatch(x0.getValue(CPos("H13")), CValue(-22.0)));
    assert (valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
