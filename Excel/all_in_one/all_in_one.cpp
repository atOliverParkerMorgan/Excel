#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>
#include "expression.h"

using namespace std::literals;
using CValue = std::variant<std::monostate, double, std::string>;

constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;
#endif /* __PROGTEST__ */


class CPos {
public:
    CPos(std::string_view str) {
        size_t index = 0;
        m_Column = 0;

        for (; !isdigit(str[index]); ++index) {
            bool isUpper = std::isupper(str[index]);
            if ((!isUpper && !std::islower(str[index])) || index + 1 >= str.size()) {
                throw std::invalid_argument("CPos argument is not valid");
            }
            const char baseChar = isUpper ? 'A' : 'a';
            m_Column *= SYSTEM_VALUE;
            m_Column += (str[index] - baseChar + 1);
        }

        if (index == 0) {
            throw std::invalid_argument("CPos argument is not valid");
        }

        m_Row = 0;
        for (; index < str.length(); ++index) {
            if (!isdigit(str[index])) {
                throw std::invalid_argument("CPos argument is not valid");
            }
            m_Row *= 10;
            m_Row += str[index] - '0';
        }
    }


    size_t getRow() const {
        return m_Row;
    }

    size_t getColumn() const {
        return m_Column;
    }

    const static size_t SYSTEM_VALUE = 26;

private:
    size_t m_Row;
    size_t m_Column;
};

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

    const static char DELIMITER = ',';
    const static char DELIMITER_OFFSET = '\\';
    const static char STRING_DELIMITER = '~';

};

using ANode = std::shared_ptr<CNode>;

class CNodeLiteral : public CNode {
public:
    virtual ~CNodeLiteral() = default;


    ASTType getType() const override {
        return LITERAL;
    }

    void moveRelative(size_t columnOffset, size_t rowOffset) override {};
};


class CReference : public CNodeLiteral {
public:
    CReference(const std::string &str);

    CReference(const CReference &other) = default;


    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    size_t getRow() const {
        return m_Row;
    }

    size_t getColumn() const {
        return m_Column;
    }

    void print(std::ostream &os) const override {
        std::string columnString;
        size_t column = m_Column;
        while (column != 0) {
            columnString += ((char) ('A' + (column % CPos::SYSTEM_VALUE) - 1));
            column /= CPos::SYSTEM_VALUE;
        }
        std::reverse(columnString.begin(), columnString.end());
        os << columnString.append(std::to_string(m_Row)) << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CReference>(*this);
    }


    void moveRelative(size_t columnOffset, size_t rowOffset) override {
        if (m_IsColumnRelative) {
            m_Column += columnOffset;
        }
        if (m_IsRowRelative) {
            m_Row += rowOffset;
        }
    }


private:
    size_t m_Row;
    bool m_IsRowRelative;
    size_t m_Column;
    bool m_IsColumnRelative;
};

class CNodeDouble : public CNodeLiteral {
public:
    CNodeDouble(double val) : m_Value(val) {}

    CNodeDouble(const CNodeDouble &other) = default;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override {
        return m_Value;
    }

    void print(std::ostream &os) const override {
        std::string stringValue = std::to_string(m_Value);
        size_t pos = stringValue.find('.');
        stringValue.erase(stringValue.find_last_not_of('0') + 1, std::string::npos);
        if (pos != std::string::npos && pos == stringValue.length() - 1) {
            stringValue.erase(pos);
        }
        os << stringValue << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CNodeDouble>(*this);
    }

private:
    double m_Value;
};


class CNodeString : public CNodeLiteral {
public:
    CNodeString(const std::string &val) : m_Value(val) {
        m_ValueDelimiterOffset = STRING_DELIMITER;
        for (size_t i = 0; i < val.length(); ++i) {
            if (val[i] == DELIMITER || val[i] == DELIMITER_OFFSET) {
                m_ValueDelimiterOffset += DELIMITER_OFFSET;
            }

            m_ValueDelimiterOffset += val[i];
        }
    }

    CNodeString(const CNodeString &other) = default;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>>& visitedReferences) const override {
        return m_Value;
    }

    ASTType getType() const override {
        return LITERAL;
    }

    void print(std::ostream &os) const override {
        os << m_ValueDelimiterOffset << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CNodeString>(*this);
    }

private:
    std::string m_Value;
    std::string m_ValueDelimiterOffset;
};

class CNodeUnaryOperand : public CNode {
public:
    void setChildrenUnary(const std::shared_ptr<CNode> &child) override {
        m_Child = child;
    }

protected:
    CNodeUnaryOperand() = default;

    CNodeUnaryOperand(const CNodeUnaryOperand &other) {
        m_Child = other.m_Child->clone();
    };

    ASTType getType() const override {
        return UNARY_OPERAND;
    }

    void moveRelative(size_t columnOffset, size_t rowOffset) override {
        m_Child->moveRelative(columnOffset, rowOffset);
    };

    ANode m_Child;
};


class CNeg : public CNodeUnaryOperand {
public:
    using CNodeUnaryOperand::CNodeUnaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    void print(std::ostream &os) const override {
        m_Child->print(os);
        os << "—" << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CNeg>(*this);
    }
};

class CNodeBinaryOperand : public CNode {
public:
    void setChildrenBinary(const std::shared_ptr<CNode> &left, const std::shared_ptr<CNode> &right) override {
        m_Left = left;
        m_Right = right;
    }

protected:
    CNodeBinaryOperand() = default;

    CNodeBinaryOperand(const CNodeBinaryOperand &other) {
        m_Left = other.m_Left->clone();
        m_Right = other.m_Right->clone();
    };

    ASTType getType() const override {
        return BINARY_OPERAND;
    }

    void moveRelative(size_t columnOffset, size_t rowOffset) override {
        m_Left->moveRelative(columnOffset, rowOffset);
        m_Right->moveRelative(columnOffset, rowOffset);
    };

    ANode m_Left = nullptr;
    ANode m_Right = nullptr;
};

class CAddition : public CNodeBinaryOperand {
public:
    using CNodeBinaryOperand::CNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "+" << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CAddition>(*this);
    }
};

class CMultiply : public CNodeBinaryOperand {
public:
    using CNodeBinaryOperand::CNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "*" << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CMultiply>(*this);
    }
};

class CDivide : public CNodeBinaryOperand {
public:
    using CNodeBinaryOperand::CNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "/" << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CDivide>(*this);
    }
};

class CSubtract : public CNodeBinaryOperand {
public:
    using CNodeBinaryOperand::CNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "-" << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CSubtract>(*this);
    }
};

class CPow : public CNodeBinaryOperand {
public:
    using CNodeBinaryOperand::CNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "^" << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CPow>(*this);
    }
};

class CEquals : public CNodeBinaryOperand {
public:
    using CNodeBinaryOperand::CNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "=" << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CEquals>(*this);
    }
};

class ASTNotEqual : public CNodeBinaryOperand {
public:
    using CNodeBinaryOperand::CNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "<>" << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<ASTNotEqual>(*this);
    }
};

class CLessThan : public CNodeBinaryOperand {
public:
    using CNodeBinaryOperand::CNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "<" << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CLessThan>(*this);
    }
};

class CGreaterThan : public CNodeBinaryOperand {
public:
    using CNodeBinaryOperand::CNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << ">" << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CGreaterThan>(*this);
    }
};

class CLessEqualThan : public CNodeBinaryOperand {
public:
    using CNodeBinaryOperand::CNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "<=" << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CLessEqualThan>(*this);
    }
};

class CGreaterEqualThan : public CNodeBinaryOperand {
public:
    using CNodeBinaryOperand::CNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                std::set<std::pair<size_t, size_t>> &visitedReferences) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << ">=" << CNode::DELIMITER;
    };

    ANode clone() const override {
        return std::make_shared<CGreaterEqualThan>(*this);
    }
};

class CBuilder : public CExprBuilder {
public:
    void opAdd() override;

    void opSub() override;

    void opMul() override;

    void opDiv() override;

    void opPow() override;

    void opNeg() override;

    void opEq() override;

    void opNe() override;

    void opLt() override;

    void opLe() override;

    void opGt() override;

    void opGe() override;

    void valNumber(double val) override;

    void valString(std::string val) override;

    void valReference(std::string val) override;

    void valRange(std::string val) override;

    void funcCall(std::string fnName,
                  int paramCount) override;

    std::shared_ptr<CNode> getRoot();
private:
    std::deque<std::shared_ptr<CNode>> m_BuilderStack;
};


CValue CPow::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                  std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::pow(std::get<double>(leftValue), std::get<double>(rightValue));
    }

    return std::monostate();
}

CValue CNeg::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                  std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue childValue = m_Child->eval(sheetNodeData, visitedReferences);
    if (std::holds_alternative<double>(childValue)) {
        return -std::get<double>(childValue);
    }

    return std::monostate();
}


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

CValue CSubtract::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                       std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) - std::get<double>(rightValue);
    }
    return std::monostate();
}

CValue CMultiply::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
                       std::set<std::pair<size_t, size_t>> &visitedReferences) const {
    CValue leftValue = m_Left->eval(sheetNodeData, visitedReferences);
    CValue rightValue = m_Right->eval(sheetNodeData, visitedReferences);

    if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
        return std::get<double>(leftValue) * std::get<double>(rightValue);
    }
    return std::monostate();
}

CValue CDivide::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
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


CValue CEquals::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
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

CValue ASTNotEqual::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
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


CValue CLessThan::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
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

CValue CGreaterThan::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
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

CValue CLessEqualThan::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
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


CValue CGreaterEqualThan::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
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

CValue CReference::eval(const std::unordered_map<std::pair<size_t, size_t>, ANode> &sheetNodeData,
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

CReference::CReference(const std::string &str) {
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
    m_BuilderStack.push_back(std::make_shared<ASTNotEqual>());
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
            if(helperStack.empty()){
                throw std::invalid_argument("Can't get root of invalid expression");
            }
            ANode right = helperStack.top();
            helperStack.pop();

            if(helperStack.empty()){
                throw std::invalid_argument("Can't get root of invalid expression");
            }
            ANode left = helperStack.top();
            helperStack.pop();

            current->setChildrenBinary(left, right);
            helperStack.push(current);

        } else if (current->getType() == UNARY_OPERAND) {
            if(helperStack.empty()){
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
    if(helperStack.empty()){
        throw std::invalid_argument("Can't get root of invalid expression");
    }
    ANode output = helperStack.top();
    helperStack.pop();
    if(!helperStack.empty()){
        throw std::invalid_argument("Can't get root of invalid expression");
    }

    return output;
}


class CSpreadsheet {
public:
    static unsigned capabilities() {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED;
    }

    CSpreadsheet() = default;

    CSpreadsheet(const CSpreadsheet &other) {
        for (auto &[key, node]: other.m_SheetNodeData) {
            m_SheetNodeData[key] = node->clone();
        }
    }

    CSpreadsheet &operator=(const CSpreadsheet &rhs) {
        if (this != &rhs) {
            for (auto &[key, node]: rhs.m_SheetNodeData) {
                m_SheetNodeData[key] = node->clone();
            }
        }
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const CSpreadsheet &cSpreadsheet) {
        for (auto &[key, node]: cSpreadsheet.m_SheetNodeData) {
            os << (char) ('A' + key.first - 1) << key.second << ": " << *node << "\n";
        }
        return os;
    }

    bool load(std::istream &is) {
        m_SheetNodeData.clear();

        bool isAtCPosFirst = true;
        bool isAtCPosSecond = false;

        std::string stringUntilDelimiter;

        std::pair<size_t, size_t> currentKey;
        try {
            while (true) {

                stringUntilDelimiter = getDataUntilDelimiter(is);
                if (is.eof()) {
                    break;
                }

                if (stringUntilDelimiter.empty()) {
                    isAtCPosFirst = true;

                    auto it = m_SheetNodeData.find(currentKey);
                    if (it == m_SheetNodeData.end()) {
                        m_SheetNodeData.insert({currentKey, m_Builder.getRoot()});
                    } else {
                        m_SheetNodeData[currentKey] = m_Builder.getRoot();
                    }
                    continue;
                }
                if (isAtCPosFirst) {
                    currentKey.first = std::stoul(stringUntilDelimiter);
                    isAtCPosFirst = false;
                    isAtCPosSecond = true;
                } else if (isAtCPosSecond) {
                    currentKey.second = std::stoul(stringUntilDelimiter);
                    isAtCPosSecond = false;
                } else {
                    if (stringUntilDelimiter[0] == CNode::STRING_DELIMITER) {
                        // ignore STRING_DELIMITER before string
                        char currentChar;
                        char nextChar;

                        is.get(currentChar);
                        is.get(nextChar);

                        std::string stringData;
                        bool foundDelimiterOffset = false;
                        while (currentChar == CNode::DELIMITER_OFFSET || nextChar != CNode::DELIMITER) {

                            // ignore first DELIMITER_OFFSET
                            if (foundDelimiterOffset || currentChar != CNode::DELIMITER_OFFSET) {
                                stringData += currentChar;
                            }

                            foundDelimiterOffset = currentChar == CNode::DELIMITER_OFFSET;
                            currentChar = nextChar;
                            is.get(nextChar);
                        }
                        stringData += currentChar;
                        m_Builder.valString(stringData);


                    } else if (isupper(stringUntilDelimiter[0]) || islower(stringUntilDelimiter[0])) {
                        m_Builder.valReference(stringUntilDelimiter);
                    } else if (isdigit(stringUntilDelimiter[0])) {
                        m_Builder.valNumber(std::stod(stringUntilDelimiter));
                    } else if (stringUntilDelimiter == "+") {
                        m_Builder.opAdd();
                    } else if (stringUntilDelimiter == "*") {
                        m_Builder.opMul();
                    } else if (stringUntilDelimiter == "-") {
                        m_Builder.opSub();
                    } else if (stringUntilDelimiter == "/") {
                        m_Builder.opDiv();
                    } else if (stringUntilDelimiter == "^") {
                        m_Builder.opPow();
                    } else if (stringUntilDelimiter == "=") {
                        m_Builder.opEq();
                    } else if (stringUntilDelimiter == "<") {
                        m_Builder.opLt();
                    } else if (stringUntilDelimiter == ">") {
                        m_Builder.opGt();
                    } else if (stringUntilDelimiter == "<=") {
                        m_Builder.opLe();
                    } else if (stringUntilDelimiter == ">=") {
                        m_Builder.opGe();
                    } else if (stringUntilDelimiter == "<>") {
                        m_Builder.opNe();
                    } else if (stringUntilDelimiter == "—") {
                        m_Builder.opNeg();
                    } else {
                        return false;
                    }
                }
            }
        } catch (const std::invalid_argument &e) {
//            std::cout << e.what() << std::endl;
            return false;
        }

        return true;
    };

    bool save(std::ostream &os) const {
        for (auto &[key, node]: m_SheetNodeData) {
            os << key.first << CNode::DELIMITER << key.second << CNode::DELIMITER << *node;
        }
        return true;
    };

    bool setCell(CPos pos, const std::string &contents) {
        parseExpression(contents, m_Builder);

        std::pair<size_t, size_t> key = {pos.getColumn(), pos.getRow()};
        auto it = m_SheetNodeData.find(key);

        if (it == m_SheetNodeData.end()) {
            m_SheetNodeData.insert({key, m_Builder.getRoot()});
        } else {
            m_SheetNodeData[key] = m_Builder.getRoot();
        }

        return true;
    }

    CValue getValue(CPos pos) {
        std::pair<size_t, size_t> key = {pos.getColumn(), pos.getRow()};

        std::set<std::pair<size_t, size_t>> visitedReferences = {key};
        auto it = m_SheetNodeData.find(key);

        if (it != m_SheetNodeData.end()) {
            try {
                return m_SheetNodeData.at(key)->eval(m_SheetNodeData, visitedReferences);
            } catch (std::runtime_error &e) {
                std::cout << e.what() << std::endl;
                return std::monostate();
            }
        }
        return std::monostate();
    }

    void copyRect(CPos dst, CPos src, int w = 1, int h = 1) {
        std::unordered_map<std::pair<size_t, size_t>, ANode> tmpCopy;
        for (size_t y = 0; y < (size_t) h; y++) {
            for (size_t x = 0; x < (size_t) w; x++) {
                std::pair<size_t, size_t> keySrc = {src.getColumn() + x, src.getRow() + y};
                std::pair<size_t, size_t> keyDest = {dst.getColumn() + x, dst.getRow() + y};

                auto it = m_SheetNodeData.find(keySrc);
                if (keyDest.first >= src.getColumn() && keyDest.first < w + src.getColumn() &&
                    keyDest.second >= src.getRow() && keyDest.second < h + src.getRow() &&
                    it != m_SheetNodeData.end()) {

                    tmpCopy[keyDest] = m_SheetNodeData[keySrc]->clone();
                } else if (it != m_SheetNodeData.end()) {
                    ANode node = m_SheetNodeData[keySrc]->clone();
                    node->moveRelative(dst.getColumn() - src.getColumn(),
                                       dst.getRow() - src.getRow());
                    m_SheetNodeData[keyDest] = node;
                }
            }
        }
        for (auto &[keyDest, node]: tmpCopy) {

            node->moveRelative(dst.getColumn() - src.getColumn(),
                               dst.getRow() - src.getRow());

            m_SheetNodeData[keyDest] = node;
        }
    };

private:
    static std::string getDataUntilDelimiter(std::istream &is) {
        std::string outputString;
        char ch;
        while (is.get(ch) && ch != CNode::DELIMITER) {
            // ignore STRING_DELIMITER logic for string literals is solved within load()
            outputString += ch;
            if (ch == CNode::STRING_DELIMITER) return outputString;

        }
        return outputString;
    }

    CBuilder m_Builder;
    std::unordered_map<std::pair<size_t, size_t>, ANode> m_SheetNodeData;
};

#ifndef __PROGTEST__

bool valueMatch(const CValue &r,
                const CValue &s) {
    if (r.index() != s.index())
        return false;
    if (r.index() == 0)
        return true;
    if (r.index() == 2)
        return std::get<std::string>(r) == std::get<std::string>(s);
    if (std::isnan(std::get<double>(r)) && std::isnan(std::get<double>(s)))
        return true;
    if (std::isinf(std::get<double>(r)) && std::isinf(std::get<double>(s)))
        return (std::get<double>(r) < 0 && std::get<double>(s) < 0)
               || (std::get<double>(r) > 0 && std::get<double>(s) > 0);
    return fabs(std::get<double>(r) - std::get<double>(s)) <= 1e8 * DBL_EPSILON * fabs(std::get<double>(r));
}

int main() {

    CSpreadsheet x0, x1;
    std::ostringstream oss;
    std::istringstream iss;
    std::string data;
    assert (x0.setCell(CPos("A1"), "10"));
    assert (x0.setCell(CPos("A2"), "20.5"));
    assert (x0.setCell(CPos("A3"), "3e1"));
    assert (x0.setCell(CPos("A4"), "=40"));
    assert (x0.setCell(CPos("A5"), "=5e+1"));
    assert (x0.setCell(CPos("A6"), "raw text with any characters, including a quote \" or a newline\n"));
    assert (x0.setCell(CPos("A7"),
                       "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\""));


    assert (valueMatch(x0.getValue(CPos("A1")), CValue(10.0)));
    assert (valueMatch(x0.getValue(CPos("A2")), CValue(20.5)));
    assert (valueMatch(x0.getValue(CPos("A3")), CValue(30.0)));
    assert (valueMatch(x0.getValue(CPos("A4")), CValue(40.0)));
    assert (valueMatch(x0.getValue(CPos("A5")), CValue(50.0)));
    assert (valueMatch(x0.getValue(CPos("A6")),
                       CValue("raw text with any characters, including a quote \" or a newline\n")));
    assert (valueMatch(x0.getValue(CPos("A7")),
                       CValue("quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++.")));
    assert (valueMatch(x0.getValue(CPos("A8")), CValue()));
    assert (valueMatch(x0.getValue(CPos("AAAA9999")), CValue()));
    assert (x0.setCell(CPos("B1"), "=A1+A2*A3"));
    assert (x0.setCell(CPos("B2"), "= -A1 ^ 2 - A2 / 2   "));
    assert (x0.setCell(CPos("B3"), "= 2 ^ $A$1"));
    assert (x0.setCell(CPos("B4"), "=($A1+A$2)^2"));
    assert (x0.setCell(CPos("B5"), "=B1+B2+B3+B4"));
    assert (x0.setCell(CPos("B6"), "=B1+B2+B3+B4+B5"));
    assert (valueMatch(x0.getValue(CPos("B1")), CValue(625.0)));
    assert (valueMatch(x0.getValue(CPos("B2")), CValue(-110.25)));
    assert (valueMatch(x0.getValue(CPos("B3")), CValue(1024.0)));
    assert (valueMatch(x0.getValue(CPos("B4")), CValue(930.25)));
    assert (valueMatch(x0.getValue(CPos("B5")), CValue(2469.0)));
    assert (valueMatch(x0.getValue(CPos("B6")), CValue(4938.0)));
    assert (x0.setCell(CPos("A1"), "12"));
    assert (valueMatch(x0.getValue(CPos("B1")), CValue(627.0)));
    assert (valueMatch(x0.getValue(CPos("B2")), CValue(-154.25)));
    assert (valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x0.getValue(CPos("B4")), CValue(1056.25)));
    assert (valueMatch(x0.getValue(CPos("B5")), CValue(5625.0)));
    assert (valueMatch(x0.getValue(CPos("B6")), CValue(11250.0)));
    x1 = x0;
    assert (x0.setCell(CPos("A2"), "100"));
    assert (x1.setCell(CPos("A2"), "=A3+A5+A4"));
    assert (valueMatch(x0.getValue(CPos("B1")), CValue(3012.0)));
    assert (valueMatch(x0.getValue(CPos("B2")), CValue(-194.0)));
    assert (valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x0.getValue(CPos("B4")), CValue(12544.0)));
    assert (valueMatch(x0.getValue(CPos("B5")), CValue(19458.0)));
    assert (valueMatch(x0.getValue(CPos("B6")), CValue(38916.0)));
    assert (valueMatch(x1.getValue(CPos("B1")), CValue(3612.0)));
    assert (valueMatch(x1.getValue(CPos("B2")), CValue(-204.0)));
    assert (valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x1.getValue(CPos("B4")), CValue(17424.0)));
    assert (valueMatch(x1.getValue(CPos("B5")), CValue(24928.0)));
    assert (valueMatch(x1.getValue(CPos("B6")), CValue(49856.0)));
    oss.clear();
    oss.str("");
    assert (x0.save(oss));
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert (x1.load(iss));
    CValue x = x1.getValue(CPos("B1"));
    CValue x2 = x1.getValue(CPos("B2"));
    CValue x3 = x1.getValue(CPos("B3"));
    assert (valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
    assert (valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
    assert (valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
    assert (valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
    assert (valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
    assert (x0.setCell(CPos("A3"), "4e1"));
    assert (valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
    assert (valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
    assert (valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
    assert (valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
    assert (valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
    oss.clear();
    oss.str("");
    assert (x0.save(oss));
    data = oss.str();
    for (size_t i = 0; i < std::min<size_t>(data.length(), 10); i++)
        data[i] ^= 0x5a;
    iss.clear();
    iss.str(data);
    assert (!x1.load(iss));
    assert (x0.setCell(CPos("D0"), "10"));
    assert (x0.setCell(CPos("D1"), "20"));
    assert (x0.setCell(CPos("D2"), "30"));
    assert (x0.setCell(CPos("D3"), "40"));
    assert (x0.setCell(CPos("D4"), "50"));
    assert (x0.setCell(CPos("E0"), "60"));
    assert (x0.setCell(CPos("E1"), "70"));
    assert (x0.setCell(CPos("E2"), "80"));
    assert (x0.setCell(CPos("E3"), "90"));
    assert (x0.setCell(CPos("E4"), "100"));
    assert (x0.setCell(CPos("F10"), "=D0+5"));
    assert (x0.setCell(CPos("F11"), "=$D0+5"));
    assert (x0.setCell(CPos("F12"), "=D$0+5"));
    assert (x0.setCell(CPos("F13"), "=$D$0+5"));
    x0.copyRect(CPos("G11"), CPos("F10"), 1, 4);
    assert (valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F14")), CValue()));
    assert (valueMatch(x0.getValue(CPos("G10")), CValue()));
    assert (valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
    assert (valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
    assert (valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
    assert (valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
    x0.copyRect(CPos("G11"), CPos("F10"), 2, 4);
    assert (valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F14")), CValue()));
    assert (valueMatch(x0.getValue(CPos("G10")), CValue()));
    assert (valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
    assert (valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
    assert (valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
    assert (valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("H10")), CValue()));
    assert (valueMatch(x0.getValue(CPos("H11")), CValue()));
    assert (valueMatch(x0.getValue(CPos("H12")), CValue()));
    assert (valueMatch(x0.getValue(CPos("H13")), CValue(35.0)));
    assert (valueMatch(x0.getValue(CPos("H14")), CValue()));
    assert (x0.setCell(CPos("F0"), "-27"));
    assert (valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));
    x0.copyRect(CPos("H12"), CPos("H13"), 1, 2);
    assert (valueMatch(x0.getValue(CPos("H12")), CValue(25.0)));
    assert (valueMatch(x0.getValue(CPos("H13")), CValue(-22.0)));
    assert (valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));


    assert (x0.setCell(CPos("A1"), "-10"));
    assert (x0.setCell(CPos("A2"), "-20.0"));
    assert (valueMatch(x0.getValue(CPos("A1")), CValue(-10.0)));
    assert (valueMatch(x0.getValue(CPos("A2")), CValue(-20.0)));
    assert (x0.setCell(CPos("A3"), "=(A1>A2)"));
    assert (valueMatch(x0.getValue(CPos("A3")), CValue(1.0)));


}

#endif /* __PROGTEST__ */
