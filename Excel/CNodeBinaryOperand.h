#pragma once
#include "CNode.h"


class CNodeBinaryOperand : public CNode {
public:
    void setChildrenBinary(const std::shared_ptr<CNode> &left, const std::shared_ptr<CNode> &right) override {
        m_Left = left;
        m_Right = right;
    }

protected:
    CNodeBinaryOperand() = default;

    CNodeBinaryOperand(const CNodeBinaryOperand &other) {
        m_Left = other.m_Left->clone();
        m_Right = other.m_Right->clone();
    };

    ASTType getType() const override {
        return BINARY_OPERAND;
    }

    void moveRelative(size_t columnOffset, size_t rowOffset) override {
        m_Left->moveRelative(columnOffset, rowOffset);
        m_Right->moveRelative(columnOffset, rowOffset);
    };

    ANode m_Left = nullptr;
    ANode m_Right = nullptr;
};