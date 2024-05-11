#pragma once
#include "CNodeLiteral.h"


class CNodeDouble : public CNodeLiteral {
public:
    CNodeDouble(double val) : m_Value(val) {}

    CNodeDouble(const CNodeDouble &other) = default;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override {
        return m_Value;
    }

    void print(std::ostream &os) const override {
        std::string stringValue = std::to_string(m_Value);
        size_t pos = stringValue.find('.');
        stringValue.erase(stringValue.find_last_not_of('0') + 1, std::string::npos);
        if (pos != std::string::npos && pos == stringValue.length() - 1) {
            stringValue.erase(pos);
        }
        os << stringValue << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CNodeDouble>(*this);
    }

private:
    double m_Value;
};