#include "AST.h"

#include <utility>

ASTNodeDouble::ASTNodeDouble(double val) : m_Value(val) {}

ASTNodeDouble::ASTNodeDouble(const ASTNodeDouble &other) : m_Value(other.m_Value) {}

ASTNodeDouble &ASTNodeDouble::operator=(const ASTNodeDouble &other) {
    if (this != &other) {
        m_Value = other.m_Value;
    }
    return *this;
}

CValue ASTNodeDouble::eval() const {
    return m_Value;
}

ASTType ASTNodeDouble::getType() const {
    return LITERAL;
}

ASTNodeString::ASTNodeString(std::string val) : m_Value(std::move(val)) {}

ASTNodeString::ASTNodeString(const ASTNodeString &other) : m_Value(other.m_Value) {}

CValue ASTNodeString::eval() const {
    return m_Value;
}

ASTType ASTNodeString::getType() const {
    return LITERAL;
}

ASTNodeOperand::ASTNodeOperand(EASTNode left, EASTNode right) : m_Left(std::move(left)), m_Right(std::move(right)) {}

ASTType ASTNodeOperand::getType() const {
    return BINARY_OPERAND;
}

void ASTNodeOperand::setChildren(const EASTNode &left, const EASTNode &right) {
    m_Left = left;
    m_Right = right;
}

ASTAddition::ASTAddition(EASTNode left, EASTNode right) : ASTNodeOperand(std::move(left), std::move(right)) {}

CValue ASTAddition::eval() const {
    return std::monostate();
}

ASTMultiply::ASTMultiply(EASTNode left, EASTNode right) : ASTNodeOperand(std::move(left), std::move(right)) {}

CValue ASTMultiply::eval() const {
    return std::monostate();
}

ASTDivide::ASTDivide(EASTNode left, EASTNode right) : ASTNodeOperand(std::move(left), std::move(right)) {}

CValue ASTDivide::eval() const {
    return std::monostate();
}

ASTSubtract::ASTSubtract(EASTNode left, EASTNode right) : ASTNodeOperand(std::move(left), std::move(right)) {}

CValue ASTSubtract::eval() const {
    return std::monostate();
}

CValue ASTRelative::eval() const {
    return std::monostate();
}

ASTType ASTRelative::getType() const {
    return LITERAL;
}

void ASTNode::setChildren(const std::shared_ptr<ASTNode> &left, const std::shared_ptr<ASTNode> &right) {}
