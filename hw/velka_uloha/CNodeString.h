#include "CNodeLiteral.h"


class CNodeString : public CNodeLiteral {
public:
    CNodeString(const std::string &val) : m_Value(val) {
        m_ValueDelimiterOffset = STRING_DELIMITER;
        for (size_t i = 0; i < val.length(); ++i) {
            if (val[i] == DELIMITER || val[i] == DELIMITER_OFFSET) {
                m_ValueDelimiterOffset += DELIMITER_OFFSET;
            }

            m_ValueDelimiterOffset += val[i];
        }
    }

    CNodeString(const CNodeString &other) = default;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>>& visitedReferences) const override {
        return m_Value;
    }

    ASTType getType() const override {
        return LITERAL;
    }

    void print(std::ostream &os) const override {
        os << m_ValueDelimiterOffset << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CNodeString>(*this);
    }

private:
    std::string m_Value;
    std::string m_ValueDelimiterOffset;
};