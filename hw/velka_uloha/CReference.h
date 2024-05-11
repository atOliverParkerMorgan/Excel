#pragma once
#include "CNodeLiteral.h"


class CReference : public CNodeLiteral {
public:
    CReference(const std::string &str);

    CReference(const CReference &other) = default;


    CValue& eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    size_t getRow() const {
        return m_Row;
    }

    size_t getColumn() const {
        return m_Column;
    }

    void print(std::ostream &os) const override;

    ANode clone() const override {
        return std::make_shared<CReference>(*this);
    }


    void moveRelative(size_t columnOffset, size_t rowOffset) override;


private:
    size_t m_Row;
    bool m_IsRowRelative;
    size_t m_Column;
    bool m_IsColumnRelative;
};