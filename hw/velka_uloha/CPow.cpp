#include "CPow.h"


CValue CPow::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                  std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::pow(std::get<double>(leftValue), std::get<double>(rightValue));
    }

    return std::monostate();
}
