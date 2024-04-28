#include "CPos.h"

#include <iostream>
#include <memory>
#include <utility>
#include <variant>
#include <unordered_map>
#include <cmath>

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

class ASTNode {
public:
    virtual ~ASTNode() = default;

    ASTNode() = default;

    virtual CValue
    eval(const std::unordered_map<std::pair<size_t, size_t>, std::shared_ptr<ASTNode> > &sheetNodeData) const = 0;

    virtual ASTType getType() const = 0;

    virtual void moveRelative(size_t columnOffset, size_t rowOffset) = 0;

    virtual void setChildrenBinary(const std::shared_ptr<ASTNode> &left, const std::shared_ptr<ASTNode> &right) {}

    virtual void setChildrenUnary(const std::shared_ptr<ASTNode> &child) {}

    virtual void print(std::ostream &os) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const ASTNode &astNode) {
        astNode.print(os);
        return os << DELIMITER;
    }

    virtual std::shared_ptr<ASTNode> clone() const = 0;

    inline const static char DELIMITER = ',';
    inline const static char DELIMITER_OFFSET = '\\';
    inline const static char STRING_DELIMITER = '~';

};

using EASTNode = std::shared_ptr<ASTNode>;

class ASTNodeLiteral : public ASTNode {
public:
    virtual ~ASTNodeLiteral() = default;


    ASTType getType() const override {
        return LITERAL;
    }

    void moveRelative(size_t columnOffset, size_t rowOffset) override {};
};


class ASTReference : public ASTNodeLiteral {
public:
    ASTReference(const std::string &str);

    ASTReference(const ASTReference &other) = default;


    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

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
        os << columnString.append(std::to_string(m_Row)) << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTReference>(*this);
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

class ASTNodeDouble : public ASTNodeLiteral {
public:
    ASTNodeDouble(double val) : m_Value(val) {}

    ASTNodeDouble(const ASTNodeDouble &other) = default;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override {
        return m_Value;
    }

    void print(std::ostream &os) const override {
        std::string stringValue = std::to_string(m_Value);
        size_t pos = stringValue.find('.');
        stringValue.erase(stringValue.find_last_not_of('0') + 1, std::string::npos);
        if (pos != std::string::npos && pos == stringValue.length() - 1) {
            stringValue.erase(pos);
        }
        os << stringValue << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTNodeDouble>(*this);
    }

private:
    double m_Value;
};


class ASTNodeString : public ASTNodeLiteral {
public:
    ASTNodeString(const std::string &val) : m_Value(val) {
        m_ValueDelimiterOffset = STRING_DELIMITER;
        for (size_t i = 0; i < val.length(); ++i) {
            if (val[i] == DELIMITER || val[i] == DELIMITER_OFFSET) {
                m_ValueDelimiterOffset += DELIMITER_OFFSET;
            }

            m_ValueDelimiterOffset += val[i];
        }
    }

    ASTNodeString(const ASTNodeString &other) = default;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override {
        return m_Value;
    }

    ASTType getType() const override {
        return LITERAL;
    }

    void print(std::ostream &os) const override {
        os << m_ValueDelimiterOffset << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTNodeString>(*this);
    }

private:
    std::string m_Value;
    std::string m_ValueDelimiterOffset;
};

class ASTNodeUnaryOperand : public ASTNode {
public:
    void setChildrenUnary(const std::shared_ptr<ASTNode> &child) override {
        m_Child = child;
    }

protected:
    ASTNodeUnaryOperand() = default;

    ASTNodeUnaryOperand(const ASTNodeUnaryOperand &other) {
        m_Child = other.m_Child->clone();
    };

    ASTType getType() const override {
        return UNARY_OPERAND;
    }

    void moveRelative(size_t columnOffset, size_t rowOffset) override {
        m_Child->moveRelative(columnOffset, rowOffset);
    };

    EASTNode m_Child;
};


class ASTNeg : public ASTNodeUnaryOperand {
public:
    using ASTNodeUnaryOperand::ASTNodeUnaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

    void print(std::ostream &os) const override {
        m_Child->print(os);
        os << "—" << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTNeg>(*this);
    }
};

class ASTNodeBinaryOperand : public ASTNode {
public:
    void setChildrenBinary(const std::shared_ptr<ASTNode> &left, const std::shared_ptr<ASTNode> &right) override {
        m_Left = left;
        m_Right = right;
    }

protected:
    ASTNodeBinaryOperand() = default;

    ASTNodeBinaryOperand(const ASTNodeBinaryOperand &other) {
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

    EASTNode m_Left = nullptr;
    EASTNode m_Right = nullptr;
};

class ASTAddition : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "+" << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTAddition>(*this);
    }
};

class ASTMultiply : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "*" << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTMultiply>(*this);
    }
};

class ASTDivide : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "/" << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTDivide>(*this);
    }
};

class ASTSubtract : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "-" << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTSubtract>(*this);
    }
};

class ASTPow : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "^" << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTPow>(*this);
    }
};

class ASTEquals : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "=" << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTEquals>(*this);
    }
};

class ASTNotEqual : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "<>" << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTNotEqual>(*this);
    }
};

class ASTLessThan : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "<" << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTLessThan>(*this);
    }
};

class ASTGreaterThan : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << ">" << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTGreaterThan>(*this);
    }
};

class ASTLessEqualThan : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << "<=" << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTLessEqualThan>(*this);
    }
};

class ASTGreaterEqualThan : public ASTNodeBinaryOperand {
public:
    using ASTNodeBinaryOperand::ASTNodeBinaryOperand;

    CValue eval(const std::unordered_map<std::pair<size_t, size_t>, EASTNode> &sheetNodeData) const override;

    void print(std::ostream &os) const override {
        m_Left->print(os);
        m_Right->print(os);
        os << ">=" << ASTNode::DELIMITER;
    };

    EASTNode clone() const override {
        return std::make_shared<ASTGreaterEqualThan>(*this);
    }
};
