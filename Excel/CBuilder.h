#pragma once
#include "CAddition.h"
#include "CSubtract.h"
#include "CMultiply.h"
#include "CDivide.h"
#include "CPow.h"
#include "CNeg.h"
#include "CNotEqual.h"
#include "CEquals.h"
#include "CNotEqual.h"
#include "CLessThan.h"
#include "CLessEqualThan.h"
#include "CGreaterThan.h"
#include "CGreaterEqualThan.h"
#include "CNodeDouble.h"
#include "CNodeString.h"
#include "CReference.h"
#include "expression.h"


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

    ANode getRoot();
private:
    std::deque<ANode> m_BuilderStack;
};

