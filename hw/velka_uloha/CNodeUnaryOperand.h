#pragma once
#include "CNode.h"


class CNodeUnaryOperand : public CNode {
public:
    void setChildrenUnary(const std::shared_ptr<CNode> &child) override {
        m_Child = child;
    }

protected:
    CNodeUnaryOperand() = default;

    CNodeUnaryOperand(const CNodeUnaryOperand &other) {
        m_Child = other.m_Child->clone();
    };

    ASTType getType() const override {
        return UNARY_OPERAND;
    }

    void moveRelative(size_t columnOffset, size_t rowOffset) override {
        m_Child->moveRelative(columnOffset, rowOffset);
    };

    ANode m_Child;
};

