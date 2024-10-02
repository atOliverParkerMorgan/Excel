add_executable(all_in_one all_in_one.cpp
)

# Add compiler flags
target_compile_options(all_in_one PRIVATE -Wall -pedantic -g -fsanitize=address)

# Include directories
target_include_directories(all_in_one PRIVATE .)

# Link against libexpression_parser.a and libasan
target_link_directories(all_in_one PRIVATE ../x86_64-linux-gnu)
target_link_libraries(all_in_one PRIVATE expression_parser asan)