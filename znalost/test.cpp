#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>

using namespace std;
class CPos {
public:
    CPos(std::string_view str) {
        size_t index = 0;
        m_Column = 0;

        for (; !isdigit(str[index]); ++index) {
            bool isUpper = std::isupper(str[index]);
            if ((!isUpper && !std::islower(str[index]) || index + 1 >= str.size())) {
                throw std::invalid_argument("CPos argument is not valid");
            }
            const char baseChar = isUpper ? 'A' : 'a';
            m_Column *= systemValue;
            m_Column += (str[index] - baseChar + 1);
        }

        if (index == 0) {
            throw std::invalid_argument("CPos argument is not valid");
        }

        m_Row = std::stoul(std::string(str.substr(index)));
    }


    size_t getRow() const {
        return m_Row;
    }

    size_t getColumn() const {
        return m_Column;
    }


private:
    size_t m_Row;
    size_t m_Column;
    inline static int systemValue = 26;
};
int main ( void )
{
    CPos("A1");
    CPos c = CPos("BA154");
    CPos("AA");
    return 0;
}