#include "CNodeUnaryOperand.h"


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