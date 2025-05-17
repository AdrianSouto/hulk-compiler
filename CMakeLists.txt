cmake_minimum_required(VERSION 3.10)
project(hulk-compiler)

set(CMAKE_CXX_STANDARD 17)


set(LLVM_DIR "C:/Program Files/LLVM/lib/cmake/llvm")
# Find LLVM

# Find LLVM (no hardcoded paths)
find_package(LLVM REQUIRED CONFIG)




# Include LLVM headers
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Add flex and bison
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

FLEX_TARGET(Scanner lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

# Source files
set(SOURCE_FILES
        main.cpp
        ASTNode.cpp
        LLVMCodeGenVisitor.cpp
        ${FLEX_Scanner_OUTPUTS}
        ${BISON_Parser_OUTPUTS}
)

add_executable(hulk-compiler ${SOURCE_FILES})

# Link against LLVM libraries
llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(hulk-compiler ${llvm_libs})
