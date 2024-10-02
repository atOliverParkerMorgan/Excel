#pragma once
#include "CNode.h"


class CNodeLiteral : public CNode {
public:
    virtual ~CNodeLiteral() = default;


    ASTType getType() const override {
        return LITERAL;
    }

    void moveRelative(size_t columnOffset, size_t rowOffset) override {};
};