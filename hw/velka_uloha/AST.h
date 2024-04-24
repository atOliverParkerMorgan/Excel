//
// Created by olixandr on 24.4.24.
//

#ifndef AST_H
#define AST_H

#include <memory> // Include necessary header
#include <utility>// For std::shared_ptr
#include <variant>
using CValue = std::variant<std::monostate, double, std::string>;

class ASTNode {
public:
    virtual ~ASTNode() = default;

    virtual CValue eval() const = 0;
};
using EASTNode = std::shared_ptr<ASTNode>;

class ASTRelative : public ASTNode {
};

class ASTNodeDouble : public ASTNode {
public:
    ASTNodeDouble(CValue value) : m_Value(value) {}

    CValue eval() const override {
        return m_Value;
    }

private:
    CValue m_Value;
};

class ASTNodeString : public ASTNode {
public:
    ASTNodeString(const std::string &value) : m_Value(value) {}

    CValue eval() const override {

        return std::monostate();
    }

private:
    std::string m_Value;
};

class ASTAddition : public ASTNode {
public:
    ASTAddition(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right)
            : m_Left(std::move(left)), m_Right(std::move(right)) {}

    CValue eval() const override {
        // return m_Left->eval() + m_Right->eval();
        return std::monostate();

    }

private:
    std::shared_ptr<ASTNode> m_Left;
    std::shared_ptr<ASTNode> m_Right;
};

class ASTMultiply : public ASTNode {
public:
    ASTMultiply(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right)
            : m_Left(std::move(left)), m_Right(std::move(right)) {}

    CValue eval() const override {
        // return m_Left->eval() * m_Right->eval();
        return std::monostate();

    }

private:
    std::shared_ptr<ASTNode> m_Left;
    std::shared_ptr<ASTNode> m_Right;
};

class ASTDivide : public ASTNode {
public:
    ASTDivide(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right)
            : m_Left(std::move(left)), m_Right(std::move(right)) {}

    CValue eval() const override {
        // Add check for division by zero
        return std::monostate();

    }

private:
    std::shared_ptr<ASTNode> m_Left;
    std::shared_ptr<ASTNode> m_Right;
};

class ASTSubtract : public ASTNode {
public:
    ASTSubtract(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right)
            : m_Left(std::move(left)), m_Right(right) {}

    CValue eval() const override {
        // return m_Left->eval() - m_Right->eval();
        return std::monostate();

    }

private:
    std::shared_ptr<ASTNode> m_Left;
    std::shared_ptr<ASTNode> m_Right;
};

#endif // AST_H

