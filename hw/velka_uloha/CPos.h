#include <string_view>
#include <cctype>
#include <stdexcept>

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