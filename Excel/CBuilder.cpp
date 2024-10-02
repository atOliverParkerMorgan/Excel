#include "CBuilder.h"


void CBuilder::opAdd() {
    m_BuilderStack.push_back(std::make_shared<CAddition>());
}

void CBuilder::opSub() {
    m_BuilderStack.push_back(std::make_shared<CSubtract>());
}

void CBuilder::opMul() {
    m_BuilderStack.push_back(std::make_shared<CMultiply>());
}

void CBuilder::opDiv() {
    m_BuilderStack.push_back(std::make_shared<CDivide>());
}

void CBuilder::opPow() {
    m_BuilderStack.push_back(std::make_shared<CPow>());
}

void CBuilder::opNeg() {
    m_BuilderStack.push_back(std::make_shared<CNeg>());
}

void CBuilder::opEq() {
    m_BuilderStack.push_back(std::make_shared<CEquals>());
}

void CBuilder::opNe() {
    m_BuilderStack.push_back(std::make_shared<CNotEqual>());
}

void CBuilder::opLt() {
    m_BuilderStack.push_back(std::make_shared<CLessThan>());
}

void CBuilder::opLe() {
    m_BuilderStack.push_back(std::make_shared<CLessEqualThan>());
}

void CBuilder::opGt() {
    m_BuilderStack.push_back(std::make_shared<CGreaterThan>());
}

void CBuilder::opGe() {
    m_BuilderStack.push_back(std::make_shared<CGreaterEqualThan>());
}

void CBuilder::valNumber(double val) {
    m_BuilderStack.push_back(std::make_shared<CNodeDouble>(val));
}

void CBuilder::valString(std::string val) {
    m_BuilderStack.push_back(std::make_shared<CNodeString>(val));
}

void CBuilder::valReference(std::string val) {
    m_BuilderStack.push_back(std::make_shared<CReference>(val));
}

void CBuilder::valRange(std::string val) {}

void CBuilder::funcCall(std::string fnName, int paramCount) {}

std::shared_ptr<CNode> CBuilder::getRoot() {

    std::stack<ANode> helperStack;
    while (!m_BuilderStack.empty()) {
        ANode current = m_BuilderStack.front();
        m_BuilderStack.pop_front();

        if (current->getType() == BINARY_OPERAND) {
            if (helperStack.empty()) {
                throw std::invalid_argument("Can't get root of invalid expression");
            }
            ANode right = helperStack.top();
            helperStack.pop();

            if (helperStack.empty()) {
                throw std::invalid_argument("Can't get root of invalid expression");
            }
            ANode left = helperStack.top();
            helperStack.pop();

            current->setChildrenBinary(left, right);
            helperStack.push(current);

        } else if (current->getType() == UNARY_OPERAND) {
            if (helperStack.empty()) {
                throw std::invalid_argument("Can't get root of invalid expression");
            }
            ANode child = helperStack.top();
            helperStack.pop();

            current->setChildrenUnary(child);
            helperStack.push(current);

        } else if (current->getType() == LITERAL) {
            helperStack.push(current);
        }
    }
    if (helperStack.empty()) {
        throw std::invalid_argument("Can't get root of invalid expression");
    }
    ANode output = helperStack.top();
    helperStack.pop();
    if (!helperStack.empty()) {
        throw std::invalid_argument("Can't get root of invalid expression");
    }

    return output;
}
