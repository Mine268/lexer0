cmake_minimum_required(VERSION 3.16)
project(lexer0)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_library(bit_flagger STATIC src/bit_flagger.cpp)
add_library(dfa STATIC src/dfa.cpp)
target_link_libraries(dfa bit_flagger)
add_library(nfa STATIC src/nfa.cpp)
target_link_libraries(nfa bit_flagger)

add_library(reg_expr STATIC src/reg_expr.cpp)

add_executable(lexer0 src/lexer0.cpp)
target_link_libraries(lexer0 reg_expr)