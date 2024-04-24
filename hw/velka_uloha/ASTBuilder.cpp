#include "ASTBuilder.h"

void ASTBuilder::opAdd() {
    m_BuilderStack.push(std::make_shared<ASTAddition>(nullptr, nullptr));
}

void ASTBuilder::opSub() {
    m_BuilderStack.push(std::make_shared<ASTSubtract>(nullptr, nullptr));
}

void ASTBuilder::opMul() {
    m_BuilderStack.push(std::make_shared<ASTMultiply>(nullptr, nullptr));
}

void ASTBuilder::opDiv() {
    m_BuilderStack.push(std::make_shared<ASTDivide>(nullptr, nullptr));
}

void ASTBuilder::opPow() {}

void ASTBuilder::opNeg() {}

void ASTBuilder::opEq() {
    std::cout << '=';
}

void ASTBuilder::opNe() {
    std::cout << '!';
}

void ASTBuilder::opLt() {
    std::cout << '<';
}

void ASTBuilder::opLe() {
    std::cout << "<=";
}

void ASTBuilder::opGt() {
    std::cout << ">";
}

void ASTBuilder::opGe() {
    std::cout << ">=";
}

void ASTBuilder::valNumber(double val) {
    m_BuilderStack.emplace(std::make_shared<ASTNodeDouble>(val));
}

void ASTBuilder::valString(std::string val) {
    m_BuilderStack.emplace(std::make_shared<ASTNodeString>(val));
}

void ASTBuilder::valReference(std::string val) {}

void ASTBuilder::valRange(std::string val) {}

void ASTBuilder::funcCall(std::string fnName, int paramCount) {}

std::shared_ptr<ASTNode> ASTBuilder::getRoot() {

    std::stack<EASTNode> helperStack;
    while (!m_BuilderStack.empty()) {
        EASTNode current = m_BuilderStack.top();
        m_BuilderStack.pop();

        if (current->getType() == BINARY_OPERAND) {
            EASTNode left = helperStack.top();
            helperStack.pop();
            EASTNode right = helperStack.top();
            helperStack.pop();
            current->setChildren(left, right);
            helperStack.push(current);

        } else if (current->getType() == UNARY_OPERAND) {
            EASTNode left = helperStack.top();
            helperStack.pop();
            current->setChildren(left, nullptr);
            helperStack.push(current);

        } else if (current->getType() == LITERAL) {
            helperStack.push(current);
        }
    }
    return helperStack.top();
}
