#include "AST.h"
#include <variant>
#include "cmath"

CValue ASTPow::eval() const {
    CValue leftValue = m_Left->eval();
    CValue rightValue = m_Right->eval();

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::pow(std::get<double>(leftValue), std::get<double>(rightValue));
    }

    return std::monostate();
}

CValue ASTNeg::eval() const {
    CValue childValue = m_Child->eval();
    if (std::holds_alternative<double>(childValue)) {
        return -std::get<double>(childValue);
    }

    return std::monostate();
}


CValue ASTAddition::eval() const {
    CValue leftValue = m_Left->eval();
    CValue rightValue = m_Right->eval();

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) + std::get<double>(rightValue);
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::get<std::string>(leftValue).append(std::get<std::string>(rightValue));
    }
    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::to_string(std::get<double>(leftValue)).append(std::get<std::string>(rightValue));
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<std::string>(leftValue).append(std::to_string(std::get<double>(rightValue)));
    }

    return std::monostate();
}

CValue ASTSubtract::eval() const {
    CValue leftValue = m_Left->eval();
    CValue rightValue = m_Right->eval();

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) - std::get<double>(rightValue);
    }
    return std::monostate();
}

CValue ASTMultiply::eval() const {
    CValue leftValue = m_Left->eval();
    CValue rightValue = m_Right->eval();

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) * std::get<double>(rightValue);
    }
    return std::monostate();
}

CValue ASTDivide::eval() const {
    CValue rightValue = m_Right->eval();
    CValue leftValue = m_Left->eval();

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) / std::get<double>(rightValue);
    }
    return std::monostate();
}


CValue ASTEquals::eval() const {
    CValue leftValue = m_Left->eval();
    CValue rightValue = m_Right->eval();

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) == std::get<double>(rightValue) ? 1.0 : 0.0;
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::get<std::string>(leftValue) == std::get<std::string>(rightValue) ? 1.0 : 0.0;
    }
    return std::monostate();
}

CValue ASTNotEqual::eval() const {
    CValue leftValue = m_Left->eval();
    CValue rightValue = m_Right->eval();

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) != std::get<double>(rightValue) ? 1.0 : 0.0;
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::get<std::string>(leftValue) != std::get<std::string>(rightValue) ? 1.0 : 0.0;
    }
    return std::monostate();
}


CValue ASTLessThan::eval() const {
    CValue leftValue = m_Left->eval();
    CValue rightValue = m_Right->eval();

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) < std::get<double>(rightValue) ? 1.0 : 0.0;
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::get<std::string>(leftValue) < std::get<std::string>(rightValue) ? 1.0 : 0.0;
    }
    return std::monostate();
}

CValue ASTGreaterThan::eval() const {
    CValue leftValue = m_Left->eval();
    CValue rightValue = m_Right->eval();

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) < std::get<double>(rightValue) ? 1.0 : 0.0;
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::get<std::string>(leftValue) < std::get<std::string>(rightValue) ? 1.0 : 0.0;
    }
    return std::monostate();
}

CValue ASTLessEqualThan::eval() const {
    CValue leftValue = m_Left->eval();
    CValue rightValue = m_Right->eval();

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) <= std::get<double>(rightValue) ? 1.0 : 0.0;
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::get<std::string>(leftValue) <= std::get<std::string>(rightValue) ? 1.0 : 0.0;
    }
    return std::monostate();
}


CValue ASTGreaterEqualThan::eval() const {
    CValue leftValue = m_Left->eval();
    CValue rightValue = m_Right->eval();

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) >= std::get<double>(rightValue) ? 1.0 : 0.0;
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::get<std::string>(leftValue) >= std::get<std::string>(rightValue) ? 1.0 : 0.0;
    }
    return std::monostate();
}

CValue ASTReference::eval() const {
    return std::monostate();
}
