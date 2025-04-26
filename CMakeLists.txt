cmake_minimum_required(VERSION 3.10)
project(FlexBisonProject)

set(CMAKE_CXX_STANDARD 17) # Establecer el estándar de C++ a C++17
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(Lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

add_executable(FlexBisonProject main.cpp ${BISON_Parser_OUTPUTS} ${FLEX_Lexer_OUTPUTS}
        ASTNode.hpp
        ASTNode.cpp)
target_include_directories(FlexBisonProject PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
