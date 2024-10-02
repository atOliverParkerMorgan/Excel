# CMakeLists.txt in the root directory

cmake_minimum_required(VERSION 3.10)
project(Pa2)

# Set the C++ compiler explicitly
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_STANDARD 20)

# Add subdirectories for each project


add_subdirectory(Excel)
add_subdirectory(Excel/all_in_one)




add_compile_options(-Wall -pedantic -Wno-long-long -O2 -c -Werror -o)
