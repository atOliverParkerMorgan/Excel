# Add source files
set(SOURCES
        main.cpp
        CNode.cpp
        CBuilder.cpp
        CPos.cpp
        CPos.h
        CNodeLiteral.cpp
        CNodeLiteral.h
        CReference.cpp
        CReference.h
        CNodeDouble.cpp
        CNodeDouble.h
        CNodeString.cpp
        CNodeString.h
        CNodeUnaryOperand.cpp
        CNodeUnaryOperand.h
        CNeg.cpp
        CNeg.h
        CNodeBinaryOperand.cpp
        CNodeBinaryOperand.h
        CAddition.cpp
        CAddition.h
        CMultiply.cpp
        CMultiply.h
        CDivide.cpp
        CDivide.h
        CSubtract.cpp
        CSubtract.h

        CEquals.cpp
        CEquals.h
        CLessThan.cpp
        CLessThan.h
        CGreaterThan.cpp
        CGreaterThan.h
        CLessEqualThan.cpp
        CLessEqualThan.h
        CGreaterEqualThan.cpp
        CGreaterEqualThan.h
        CPow.cpp
        CPow.h
        CNotEqual.cpp
        CSpreadSheet.cpp
        CSpreadSheet.h
)

# Add executable target
add_executable(velka_uloha ${SOURCES})

# Add compiler flags
target_compile_options(velka_uloha PRIVATE -Wall -pedantic -g -fsanitize=address)

# Include directories
target_include_directories(velka_uloha PRIVATE .)

# Link against libexpression_parser.a and libasan
target_link_directories(velka_uloha PRIVATE x86_64-linux-gnu)
target_link_libraries(velka_uloha PRIVATE expression_parser asan)