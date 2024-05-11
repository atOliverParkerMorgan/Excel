#include "CBuilder.h"


class CSpreadsheet {
public:
    static unsigned capabilities() {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED;
    }

    CSpreadsheet() = default;

    CSpreadsheet(const CSpreadsheet &other) {
        for (auto &[key, node]: other.m_SheetNodeData) {
            m_SheetNodeData[key] = node->clone();
        }
    }

    CSpreadsheet &operator=(const CSpreadsheet &rhs) {
        if (this != &rhs) {
            for (auto &[key, node]: rhs.m_SheetNodeData) {
                m_SheetNodeData[key] = node->clone();
            }
        }
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const CSpreadsheet &cSpreadsheet) {
        for (auto &[key, node]: cSpreadsheet.m_SheetNodeData) {
            os << (char) ('A' + key.first - 1) << key.second << ": " << *node << "\n";
        }
        return os;
    }

    bool load(std::istream &is) {
        m_SheetNodeData.clear();

        bool isAtCPosFirst = true;
        bool isAtCPosSecond = false;

        std::string stringUntilDelimiter;

        std::pair<size_t, size_t> currentKey;
        try {
            while (true) {

                stringUntilDelimiter = getDataUntilDelimiter(is);
                if (is.eof()) {
                    break;
                }

                if (stringUntilDelimiter.empty()) {
                    isAtCPosFirst = true;

                    auto it = m_SheetNodeData.find(currentKey);
                    if (it == m_SheetNodeData.end()) {
                        m_SheetNodeData.insert({currentKey, m_Builder.getRoot()});
                    } else {
                        m_SheetNodeData[currentKey] = m_Builder.getRoot();
                    }
                    continue;
                }
                if (isAtCPosFirst) {
                    currentKey.first = std::stoul(stringUntilDelimiter);
                    isAtCPosFirst = false;
                    isAtCPosSecond = true;
                } else if (isAtCPosSecond) {
                    currentKey.second = std::stoul(stringUntilDelimiter);
                    isAtCPosSecond = false;
                } else {
                    if (stringUntilDelimiter[0] == CNode::STRING_DELIMITER) {
                        // ignore STRING_DELIMITER before string
                        char currentChar;
                        char nextChar;

                        is.get(currentChar);
                        is.get(nextChar);

                        std::string stringData;
                        bool foundDelimiterOffset = false;
                        while (currentChar == CNode::DELIMITER_OFFSET || nextChar != CNode::DELIMITER) {

                            // ignore first DELIMITER_OFFSET
                            if (foundDelimiterOffset || currentChar != CNode::DELIMITER_OFFSET) {
                                stringData += currentChar;
                            }

                            foundDelimiterOffset = currentChar == CNode::DELIMITER_OFFSET;
                            currentChar = nextChar;
                            is.get(nextChar);
                        }
                        stringData += currentChar;
                        m_Builder.valString(stringData);


                    } else if (isupper(stringUntilDelimiter[0]) || islower(stringUntilDelimiter[0])) {
                        m_Builder.valReference(stringUntilDelimiter);
                    } else if (isdigit(stringUntilDelimiter[0])) {
                        m_Builder.valNumber(std::stod(stringUntilDelimiter));
                    } else if (stringUntilDelimiter == "+") {
                        m_Builder.opAdd();
                    } else if (stringUntilDelimiter == "*") {
                        m_Builder.opMul();
                    } else if (stringUntilDelimiter == "-") {
                        m_Builder.opSub();
                    } else if (stringUntilDelimiter == "/") {
                        m_Builder.opDiv();
                    } else if (stringUntilDelimiter == "^") {
                        m_Builder.opPow();
                    } else if (stringUntilDelimiter == "=") {
                        m_Builder.opEq();
                    } else if (stringUntilDelimiter == "<") {
                        m_Builder.opLt();
                    } else if (stringUntilDelimiter == ">") {
                        m_Builder.opGt();
                    } else if (stringUntilDelimiter == "<=") {
                        m_Builder.opLe();
                    } else if (stringUntilDelimiter == ">=") {
                        m_Builder.opGe();
                    } else if (stringUntilDelimiter == "<>") {
                        m_Builder.opNe();
                    } else if (stringUntilDelimiter == "—") {
                        m_Builder.opNeg();
                    } else {
                        return false;
                    }
                }
            }
        } catch (const std::invalid_argument &e) {
            return false;
        }

        return true;
    };

    bool save(std::ostream &os) const {
        for (auto &[key, node]: m_SheetNodeData) {
            os << key.first << CNode::DELIMITER << key.second << CNode::DELIMITER << *node;
        }
        return true;
    };

    bool setCell(CPos pos, std::string contents) {
        parseExpression(std::move(contents), m_Builder);

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

        std::set<std::pair<size_t, size_t>> visitedReferences = {key};
        auto it = m_SheetNodeData.find(key);

        if (it != m_SheetNodeData.end()) {
            try {
                return m_SheetNodeData.at(key)->eval(m_SheetNodeData, visitedReferences);
            } catch (std::runtime_error &e) {
                return std::monostate();
            }
        }
        return std::monostate();
    }

    void copyRect(CPos dst, CPos src, int w = 1, int h = 1) {
        std::unordered_map<std::pair<size_t, size_t>, ANode> tmpCopy;
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
                    ANode node = m_SheetNodeData[keySrc]->clone();
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
    static std::string getDataUntilDelimiter(std::istream &is) {
        std::string outputString;
        char ch;
        while (is.get(ch) && ch != CNode::DELIMITER) {
            // ignore STRING_DELIMITER logic for string literals is solved within load()
            outputString += ch;
            if (ch == CNode::STRING_DELIMITER) return outputString;

        }
        return outputString;
    }

    CBuilder m_Builder;
    std::unordered_map<std::pair<size_t, size_t>, ANode> m_SheetNodeData;
};
