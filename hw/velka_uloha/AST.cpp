#include "AST.h"
#include <variant>
#include "cmath"

CValue ASTPow::eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData,
                    std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::pow(std::get<double>(leftValue), std::get<double>(rightValue));
    }

    return std::monostate();
}

CValue ASTNeg::eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData,
                    std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue childValue = m_Child->eval(sheetNodeData, visitedReferences);
    if (std::holds_alternative<double>(childValue)) {
        return -std::get<double>(childValue);
    }

    return std::monostate();
}


CValue ASTAddition::eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData,
                         std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

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

CValue ASTSubtract::eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData,
                         std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) - std::get<double>(rightValue);
    }
    return std::monostate();
}

CValue ASTMultiply::eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData,
                         std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) * std::get<double>(rightValue);
    }
    return std::monostate();
}

CValue ASTDivide::eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData,
                       std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);
    bool isRightDouble = std::holds_alternative<double>(rightValue);

    if (isRightDouble && std::get<double>(rightValue) == 0) {
        return std::monostate();
    }

    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) / std::get<double>(rightValue);
    }
    return std::monostate();
}


CValue ASTEquals::eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData,
                       std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) == std::get<double>(rightValue) ? 1.0 : 0.0;
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::get<std::string>(leftValue) == std::get<std::string>(rightValue) ? 1.0 : 0.0;
    }
    return std::monostate();
}

CValue ASTNotEqual::eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData,
                         std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) != std::get<double>(rightValue) ? 1.0 : 0.0;
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::get<std::string>(leftValue) != std::get<std::string>(rightValue) ? 1.0 : 0.0;
    }
    return std::monostate();
}


CValue ASTLessThan::eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData,
                         std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) < std::get<double>(rightValue) ? 1.0 : 0.0;
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::get<std::string>(leftValue) < std::get<std::string>(rightValue) ? 1.0 : 0.0;
    }
    return std::monostate();
}

CValue ASTGreaterThan::eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData,
                            std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) > std::get<double>(rightValue) ? 1.0 : 0.0;
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::get<std::string>(leftValue) > std::get<std::string>(rightValue) ? 1.0 : 0.0;
    }
    return std::monostate();
}

CValue ASTLessEqualThan::eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData,
                              std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) <= std::get<double>(rightValue) ? 1.0 : 0.0;
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::get<std::string>(leftValue) <= std::get<std::string>(rightValue) ? 1.0 : 0.0;
    }
    return std::monostate();
}


CValue ASTGreaterEqualThan::eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData,
                                 std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) >= std::get<double>(rightValue) ? 1.0 : 0.0;
    }
    if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
        return std::get<std::string>(leftValue) >= std::get<std::string>(rightValue) ? 1.0 : 0.0;
    }
    return std::monostate();
}

CValue ASTReference::eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData,
                          std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    std::pair<size_t, size_t> key = {getColumn(), getRow()};
    auto it = sheetNodeData.find(key);

    if (visitedReferences.contains(key)) {
        throw std::runtime_error("Expression contains a cycle.");
    }

    if (it != sheetNodeData.end()) {
        visitedReferences.insert(key);
        CValue output = sheetNodeData.at(key)->eval(sheetNodeData, visitedReferences);
        visitedReferences.erase(key);
        return output;
    }

    return std::monostate();
}

ASTReference::ASTReference(const std::string &str) {
    size_t index = 0;
    m_Column = 0;

    m_IsColumnRelative = str[0] != '$';
    if (!m_IsColumnRelative) ++index;

    for (; !isdigit(str[index]); ++index) {
        m_IsRowRelative = str[index] != '$';
        if (!m_IsRowRelative) {
            ++index;
            break;
        }

        bool isUpper = std::isupper(str[index]);
        const char baseChar = isUpper ? 'A' : 'a';
        m_Column *= CPos::SYSTEM_VALUE;
        m_Column += (str[index] - baseChar + 1);
    }

    m_Row = std::stoul(std::string(str.substr(index)));
}
