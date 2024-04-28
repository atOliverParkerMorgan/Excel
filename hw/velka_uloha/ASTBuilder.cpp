#include "ASTBuilder.h"

void ASTBuilder::opAdd() {
    m_BuilderStack.push_back(std::make_shared<ASTAddition>());
}

void ASTBuilder::opSub() {
    m_BuilderStack.push_back(std::make_shared<ASTSubtract>());
}

void ASTBuilder::opMul() {
    m_BuilderStack.push_back(std::make_shared<ASTMultiply>());
}

void ASTBuilder::opDiv() {
    m_BuilderStack.push_back(std::make_shared<ASTDivide>());
}

void ASTBuilder::opPow() {
    m_BuilderStack.push_back(std::make_shared<ASTPow>());
}

void ASTBuilder::opNeg() {
    m_BuilderStack.push_back(std::make_shared<ASTNeg>());
}

void ASTBuilder::opEq() {
    m_BuilderStack.push_back(std::make_shared<ASTEquals>());
}

void ASTBuilder::opNe() {
    m_BuilderStack.push_back(std::make_shared<ASTNotEqual>());
}

void ASTBuilder::opLt() {
    m_BuilderStack.push_back(std::make_shared<ASTLessThan>());
}

void ASTBuilder::opLe() {
    m_BuilderStack.push_back(std::make_shared<ASTLessEqualThan>());
}

void ASTBuilder::opGt() {
    m_BuilderStack.push_back(std::make_shared<ASTGreaterThan>());
}

void ASTBuilder::opGe() {
    m_BuilderStack.push_back(std::make_shared<ASTGreaterEqualThan>());
}

void ASTBuilder::valNumber(double val) {
    m_BuilderStack.push_back(std::make_shared<ASTNodeDouble>(val));
}

void ASTBuilder::valString(std::string val) {
    m_BuilderStack.push_back(std::make_shared<ASTNodeString>(val));
}

void ASTBuilder::valReference(std::string val) {
    m_BuilderStack.push_back(std::make_shared<ASTReference>(val));
}

void ASTBuilder::valRange(std::string val) {}

void ASTBuilder::funcCall(std::string fnName, int paramCount) {}

std::shared_ptr<ASTNode> ASTBuilder::getRoot() {

    std::stack<EASTNode> helperStack;
    while (!m_BuilderStack.empty()) {
        EASTNode current = m_BuilderStack.front();
        m_BuilderStack.pop_front();

        if (current->getType() == BINARY_OPERAND) {
            if(helperStack.empty()){
                throw std::invalid_argument("Can't get root of invalid expression");
            }
            EASTNode right = helperStack.top();
            helperStack.pop();

            if(helperStack.empty()){
                throw std::invalid_argument("Can't get root of invalid expression");
            }
            EASTNode left = helperStack.top();
            helperStack.pop();

            current->setChildrenBinary(left, right);
            helperStack.push(current);

        } else if (current->getType() == UNARY_OPERAND) {
            if(helperStack.empty()){
                throw std::invalid_argument("Can't get root of invalid expression");
            }
            EASTNode child = helperStack.top();
            helperStack.pop();

            current->setChildrenUnary(child);
            helperStack.push(current);

        } else if (current->getType() == LITERAL) {
            helperStack.push(current);
        }
    }
    if(helperStack.empty()){
        throw std::invalid_argument("Can't get root of invalid expression");
    }
    EASTNode output = helperStack.top();
    helperStack.pop();
    if(!helperStack.empty()){
        throw std::invalid_argument("Can't get root of invalid expression");
    }

    return output;
}
