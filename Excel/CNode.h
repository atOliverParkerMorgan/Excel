#pragma once
#include "CPos.h"

using namespace std;

namespace std {
    template<>
    struct hash<pair<size_t, size_t>> {
        size_t operator()(const pair<size_t, size_t> &p) const {
            return (p.first << 32) | (p.second);
        }
    };
}

using CValue = std::variant<std::monostate, double, std::string>;

enum ASTType {
    UNARY_OPERAND,
    BINARY_OPERAND,
    LITERAL
};

class CNode {
public:
    virtual ~CNode() = default;

    CNode() = default;

    virtual CValue
    eval(const std::unordered_map<std::pair<size_t, size_t>, std::shared_ptr<CNode> > &sheetNodeData,
         std::set<std::pair<size_t, size_t>> &visitedReferences) const = 0;

    virtual ASTType getType() const = 0;

    virtual void moveRelative(size_t columnOffset, size_t rowOffset) = 0;

    virtual void setChildrenBinary(const std::shared_ptr<CNode> &left, const std::shared_ptr<CNode> &right) {}

    virtual void setChildrenUnary(const std::shared_ptr<CNode> &child) {}

    virtual void print(std::ostream &os) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const CNode &astNode) {
        astNode.print(os);
        return os << DELIMITER;
    }

    virtual std::shared_ptr<CNode> clone() const = 0;

    constexpr static char DELIMITER = ',';
    constexpr static char DELIMITER_OFFSET = '\\';
    constexpr static char STRING_DELIMITER = '~';

};

using ANode = std::shared_ptr<CNode>;
