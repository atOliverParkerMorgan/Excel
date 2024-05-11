#include "CReference.h"


CReference::CReference(const std::string &str) {
    size_t index = 0;
    m_Column = 0;

    m_IsColumnRelative = str[0] != '$';
    if (!m_IsColumnRelative) ++index;

    for (; !isdigit(str[index]); ++index) {
        m_IsRowRelative = str[index] != '$';
        if (!m_IsRowRelative) {
            ++index;
            break;
        }

        bool isUpper = std::isupper(str[index]);
        const char baseChar = isUpper ? 'A' : 'a';
        m_Column *= CPos::SYSTEM_VALUE;
        m_Column += (str[index] - baseChar + 1);
    }

    m_Row = std::stoul(std::string(str.substr(index)));
}

CValue CReference::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                        std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    std::pair<size_t, size_t> key = {getColumn(), getRow()};
    auto it = sheetNodeData.find(key);

    if (visitedReferences.contains(key)) {
        throw std::runtime_error("Expression contains a cycle.");
    }

    if (it != sheetNodeData.end()) {
        visitedReferences.insert(key);
        CValue output = sheetNodeData.at(key)->eval(sheetNodeData, visitedReferences);
        visitedReferences.erase(key);
        return output;
    }

    return std::monostate();
}

void CReference::print(std::ostream &os) const {
    std::string columnString;
    size_t column = m_Column;
    while (column != 0) {
        columnString += ((char) ('A' + (column % CPos::SYSTEM_VALUE) - 1));
        column /= CPos::SYSTEM_VALUE;
    }
    std::reverse(columnString.begin(), columnString.end());
    os << columnString.append(std::to_string(m_Row)) << CNode::DELIMITER;
}

void CReference::moveRelative(size_t columnOffset, size_t rowOffset) {
    if (m_IsColumnRelative) {
        m_Column += columnOffset;
    }
    if (m_IsRowRelative) {
        m_Row += rowOffset;
    }
}
