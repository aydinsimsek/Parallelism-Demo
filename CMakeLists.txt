cmake_minimum_required(VERSION 3.10)
project(Matrix-Multiplication)

set(CMAKE_CXX_STANDARD 11)  
set(CMAKE_CXX_FLAGS "-Wall") 

add_executable(matrix-multiplication matrix-multiplication.cpp)

if(UNIX)
    target_link_libraries(matrix-multiplication PRIVATE pthread)
endif()