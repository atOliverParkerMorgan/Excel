#include "CAddition.h"


CValue CAddition::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
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