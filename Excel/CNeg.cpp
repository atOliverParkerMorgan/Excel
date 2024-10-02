#include "CNeg.h"


CValue CNeg::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                  std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue childValue = m_Child->eval(sheetNodeData, visitedReferences);
    if (std::holds_alternative<double>(childValue)) {
        return -std::get<double>(childValue);
    }

    return std::monostate();
}