#pragma once
#include "CNodeBinaryOperand.h"


class CPow : public CNodeBinaryOperand {
public:
    using CNodeBinaryOperand::CNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "^" << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CPow>(*this);
    }
};
