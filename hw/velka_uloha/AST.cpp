// AST.h

#ifndef AST_H
#define AST_H

#include <memory> // Include necessary header
#include <utility>
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
    ASTNodeDouble(double val) : m_Value(val) {};

    CValue eval() const override {
        return std::monostate(); // Example, replace with actual logic
    }

private:
    double m_Value;
};

class ASTNodeString : public ASTNode {
public:
    ASTNodeString(std::string val) : m_Value(std::move(val)) {};

    CValue eval() const override {
        return std::monostate(); // Example, replace with actual logic
    }

private:
    std::string m_Value;
};

class ASTAddition : public ASTNode {
public:
    ASTAddition(EASTNode left, EASTNode right) : m_Left(std::move(left)), m_Right(std::move(right)) {};

    CValue eval() const override {
        return std::monostate(); // Example, replace with actual logic
    }


private:
    EASTNode m_Left;
    EASTNode m_Right;
};

class ASTMultiply : public ASTNode {
public:
    ASTMultiply(EASTNode left, EASTNode right) : m_Left(std::move(left)), m_Right(std::move(right)) {};

    CValue eval() const override {
        // Add implementation or remove override specifier
        return std::monostate(); // Example, replace with actual logic
    }

private:
    EASTNode m_Left;
    EASTNode m_Right;
};

class ASTDivide : public ASTNode {
public:
    ASTDivide(EASTNode left, EASTNode right) : m_Left(std::move(left)), m_Right(std::move(right)) {};

    CValue eval() const override {
        return std::monostate(); // Example, replace with actual logic
    }

private:
    EASTNode m_Left;
    EASTNode m_Right;
};


class ASTSubtract : public ASTNode {
public:
    ASTSubtract(EASTNode left, EASTNode right) : m_Left(std::move(left)), m_Right(std::move(right)) {};

    CValue eval() const override {
        // Add implementation or remove override specifier
        return std::monostate(); // Example, replace with actual logic
    }

private:
    EASTNode m_Left;
    EASTNode m_Right;
};

#endif // AST_H
