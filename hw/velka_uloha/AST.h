#include "CPos.h"

#include <iostream>
#include <memory>
#include <utility>
#include <variant>
#include <unordered_map>

using namespace std;

namespace std {
    template<>
    // https://www.geeksforgeeks.org/
    struct hash<pair<size_t, size_t>> {
        size_t operator()(const pair<size_t, size_t> &p) const {
            // return (p.first << 32) | (p.second);
            // option for not 64-bit
            return std::hash<size_t>{}(p.first) ^ std::hash<size_t>{}(p.second);
        }
    };
}

using CValue = std::variant<std::monostate, double, std::string>;

enum ASTType {
    UNARY_OPERAND,
    BINARY_OPERAND,
    LITERAL
};

class ASTNode {
public:
    virtual ~ASTNode() = default;

    virtual CValue
    eval(const std::unordered_map<std::pair<size_t, size_t>, std::shared_ptr<ASTNode> > &sheetNodeData) const = 0;

    virtual ASTType getType() const = 0;

    virtual void moveRelative(size_t columnOffset, size_t rowOffset) = 0;

    virtual void setChildrenBinary(const std::shared_ptr<ASTNode> &left, const std::shared_ptr<ASTNode> &right) {}

    virtual void setChildrenUnary(const std::shared_ptr<ASTNode> &child) {}
};

using EASTNode = std::shared_ptr<ASTNode>;

class ASTNodeLiteral : public ASTNode {
public:
    virtual ~ASTNodeLiteral() = default;

    ASTType getType() const override {
        return LITERAL;
    }

    void moveRelative(size_t columnOffset, size_t rowOffset) override {};

};


class ASTReference : public ASTNodeLiteral {
public:
    ASTReference(const std::string &str);

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

    size_t getRow() const {
        return m_Row;
    }

    size_t getColumn() const {
        return m_Column;
    }

    void moveRelative(size_t columnOffset, size_t rowOffset) override {
        ASTReference tmp = *this;

        if (tmp.m_IsColumnRelative) {
            tmp.m_Column += columnOffset;
        }
        if (tmp.m_IsRowRelative) {
            tmp.m_Row += rowOffset;
        }

        *this = tmp;
    }


private:
    size_t m_Row;
    bool m_IsRowRelative;
    size_t m_Column;
    bool m_IsColumnRelative;
};

class ASTNodeDouble : public ASTNodeLiteral {
public:
    ASTNodeDouble(double val) : m_Value(val) {}

    ASTNodeDouble(const ASTNodeDouble &other) : m_Value(other.m_Value) {}


    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override {
        return m_Value;
    }

private:
    double m_Value;
};

class ASTNodeString : public ASTNodeLiteral {
public:
    ASTNodeString(std::string val) : m_Value(std::move(val)) {}

    ASTNodeString(const ASTNodeString &other) : m_Value(other.m_Value) {}

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override {
        return m_Value;
    }

    ASTType getType() const override {
        return LITERAL;
    }

private:
    std::string m_Value;
};

class ASTNodeUnaryOperand : public ASTNode {
public:
    void setChildrenUnary(const std::shared_ptr<ASTNode> &child) override {
        m_Child = child;
    }

protected:
    ASTNodeUnaryOperand() = default;

    ASTNodeUnaryOperand(EASTNode child) : m_Child(std::move(child)) {}

    ASTType getType() const override {
        return UNARY_OPERAND;
    }

    void moveRelative(size_t columnOffset, size_t rowOffset) override {
        m_Child->moveRelative(columnOffset, rowOffset);
    };

    EASTNode m_Child;
};


class ASTNeg : public ASTNodeUnaryOperand {
public:
    using ASTNodeUnaryOperand::ASTNodeUnaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

};

class ASTNodeBinaryOperand : public ASTNode {
public:
    void setChildrenBinary(const std::shared_ptr<ASTNode> &left, const std::shared_ptr<ASTNode> &right) override {
        m_Left = left;
        m_Right = right;
    }

protected:
    ASTNodeBinaryOperand() = default;

    ASTType getType() const override {
        return BINARY_OPERAND;
    }

    void moveRelative(size_t columnOffset, size_t rowOffset) override {
        m_Left->moveRelative(columnOffset, rowOffset);
        m_Right->moveRelative(columnOffset, rowOffset);
    };

    EASTNode m_Left = nullptr;
    EASTNode m_Right = nullptr;
};

class ASTAddition : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

};

class ASTMultiply : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

};

class ASTDivide : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

};

class ASTSubtract : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

};

class ASTPow : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

};

class ASTEquals : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

};

class ASTNotEqual : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

};

class ASTLessThan : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

};

class ASTGreaterThan : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

};

class ASTLessEqualThan : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

};

class ASTGreaterEqualThan : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;
};
