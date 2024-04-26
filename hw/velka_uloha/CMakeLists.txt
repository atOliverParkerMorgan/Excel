# Add source files
set(SOURCES
        test.cpp
        AST.cpp
        ASTBuilder.cpp
        CPos.cpp
        CPos.h
)

# Add executable target
add_executable(velka_uloha ${SOURCES})

# Add compiler flags
target_compile_options(velka_uloha PRIVATE -Wall -pedantic -g -fsanitize=address)

# Include directories
target_include_directories(velka_uloha PRIVATE ./)

# Link against libexpression_parser.a and libasan
target_link_directories(velka_uloha PRIVATE ./x86_64-linux-gnu)
target_link_libraries(velka_uloha PRIVATE expression_parser asan)