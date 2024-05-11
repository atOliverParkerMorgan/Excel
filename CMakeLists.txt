# CMakeLists.txt in the root directory

cmake_minimum_required(VERSION 3.10)
project(Pa2)

# Set the C++ compiler explicitly
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_STANDARD 20)

# Add subdirectories for each project
add_subdirectory(cv/cv_01)
add_subdirectory(cv/cv_02)
add_subdirectory(cv/cv_03/complex)
add_subdirectory(cv/cv_04)
add_subdirectory(cv/cv_06)
add_subdirectory(cv/cv_07)
add_subdirectory(cv/cv_10)
add_subdirectory(cv/cv_11)


add_subdirectory(znalost)


add_subdirectory(hw/hw_01)
add_subdirectory(hw/hw_02)
add_subdirectory(hw/hw_03)
add_subdirectory(hw/hw_04)
add_subdirectory(hw/hw_05)
add_subdirectory(hw/hw_06)

add_subdirectory(hw/velka_uloha)
add_subdirectory(hw/velka_uloha/all_in_one)



add_subdirectory(hw_cv/cv_03)
add_subdirectory(hw_cv/cv_06)


add_compile_options(-Wall -pedantic -Wno-long-long -O2 -c -Werror -o)
