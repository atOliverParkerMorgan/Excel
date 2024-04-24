#ifndef ASTBUILDER_H
#define ASTBUILDER_H

#include "AST.h"
#include <iostream> // Include necessary header for std::cout
#include <stack>

using EASTNode = std::shared_ptr<ASTNode>;

class ASTBuilder {
public:
    void opAdd() {
        m_BuilderStack.push(std::make_shared<ASTAddition>(nullptr, nullptr));
    }

    void opSub() {
        m_BuilderStack.push(std::make_shared<ASTSubtract>(nullptr, nullptr));
    }

    void opMul() {
        m_BuilderStack.push(std::make_shared<ASTMultiply>(nullptr, nullptr));
    }

    void opDiv() {
        m_BuilderStack.push(std::make_shared<ASTDivide>(nullptr, nullptr));
    }

    void opPow() {}

    void opNeg() {}

    void opEq() {
        std::cout << '=';
    }

    void opNe() {
        std::cout << '!';
    }

    void opLt() {
        std::cout << '<';
    }

    void opLe() {
        std::cout << "<=";
    }

    void opGt() {
        std::cout << ">";
    }

    void opGe() {
        std::cout << ">=";
    }

    void valNumber(double val) {
        m_BuilderStack.push(std::make_shared<ASTNodeDouble>(val));
    }

    void valString(std::string val) {
        m_BuilderStack.push(std::make_shared<ASTNodeString>(val));
    }

    void valReference(std::string val) {}

    void valRange(std::string val) {}

    void funcCall(std::string fnName, int paramCount) {}

private:
    std::stack<EASTNode> m_BuilderStack;
};

#endif // ASTBUILDER_H
