#include <iostream>
#include <memory>
#include <utility>
#include <variant>

using namespace std;

using CValue = std::variant<std::monostate, double, std::string>;

enum ASTType {
    UNARY_OPERAND,
    BINARY_OPERAND,
    LITERAL
};

class ASTNode {
public:
    virtual ~ASTNode() = default;

    virtual CValue eval() const = 0;

    virtual ASTType getType() const = 0;

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
};

class ASTReference : public ASTNodeLiteral {
public:
    virtual ~ASTReference() = default;

    CValue eval() const override;

};

class ASTNodeDouble : public ASTNodeLiteral {
public:
    ASTNodeDouble(double val) : m_Value(val) {}

    ASTNodeDouble(const ASTNodeDouble &other) : m_Value(other.m_Value) {}


    CValue eval() const override {
        return m_Value;
    }

private:
    double m_Value;
};

class ASTNodeString : public ASTNodeLiteral {
public:
    ASTNodeString(std::string val) : m_Value(std::move(val)) {}

    ASTNodeString(const ASTNodeString &other) : m_Value(other.m_Value) {}

    CValue eval() const override {
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

    EASTNode m_Child;
};


class ASTNeg : public ASTNodeUnaryOperand {
public:
    using ASTNodeUnaryOperand::ASTNodeUnaryOperand;

    CValue eval() const override;

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

    EASTNode m_Left = nullptr;
    EASTNode m_Right = nullptr;
};

class ASTAddition : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval() const override;

};

class ASTMultiply : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval() const override;

};

class ASTDivide : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval() const override;

};

class ASTSubtract : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval() const override;

};

class ASTPow : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval() const override;

};

class ASTEquals : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval() const override;

};

class ASTNotEqual : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval() const override;

};

class ASTLessThan : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval() const override;

};

class ASTGreaterThan : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval() const override;

};

class ASTLessEqualThan : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval() const override;

};

class ASTGreaterEqualThan : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval() const override;
};
