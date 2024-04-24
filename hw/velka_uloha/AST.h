#include <memory>
#include <utility>
#include <variant>

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

    virtual void setChildren(const std::shared_ptr<ASTNode> &left, const std::shared_ptr<ASTNode> &right);
};

using EASTNode = std::shared_ptr<ASTNode>;

class ASTRelative : public ASTNode {
public:
    virtual ~ASTRelative() = default;

    CValue eval() const override;

    ASTType getType() const override;
};

class ASTNodeLiteral : public ASTNode {
public:
    virtual ~ASTNodeLiteral() = default;
};

class ASTNodeDouble : public ASTNodeLiteral {
public:
    ASTNodeDouble(double val);

    ASTNodeDouble(const ASTNodeDouble &other);

    ASTNodeDouble &operator=(const ASTNodeDouble &other);

    CValue eval() const override;

    ASTType getType() const override;


private:
    double m_Value;
};

class ASTNodeString : public ASTNodeLiteral {
public:
    ASTNodeString(std::string val);

    ASTNodeString(const ASTNodeString &other);

    CValue eval() const override;

    ASTType getType() const override;


private:
    std::string m_Value;
};

class ASTNodeOperand : public ASTNode {
public:
    void setChildren(const EASTNode &left, const EASTNode &right) override;

protected:
    ASTNodeOperand(EASTNode left, EASTNode right);

    ASTType getType() const override;

    EASTNode m_Left;
    EASTNode m_Right;
};

class ASTAddition : public ASTNodeOperand {
public:
    ASTAddition(EASTNode left, EASTNode right);

    CValue eval() const override;

};

class ASTMultiply : public ASTNodeOperand {
public:
    ASTMultiply(EASTNode left, EASTNode right);

    CValue eval() const override;

};

class ASTDivide : public ASTNodeOperand {
public:
    ASTDivide(EASTNode left, EASTNode right);

    CValue eval() const override;
};

class ASTSubtract : public ASTNodeOperand {
public:
    ASTSubtract(EASTNode left, EASTNode right);

    CValue eval() const override;
};
