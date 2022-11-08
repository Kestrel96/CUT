cmake_minimum_required(VERSION 3.6)

project(CUT)
enable_testing()
# set(CMAKE_VERBOSE_MAKEFILE ON)
set(INC_DIR ./include)
set(SRC_DIR ./src)
set(TEST_DIR ./test)

aux_source_directory(${SRC_DIR} SRC)
aux_source_directory(${TEST_DIR} TEST)
include_directories(${INC_DIR})
add_compile_options(-g -Wall -Wextra)

add_link_options(-pthread)

add_executable(cut ${SRC})

add_executable(read_test "./src/reader.c" ${TEST})
target_compile_definitions(read_test PRIVATE STAT_PATH="./xxx")

add_test(NAME read_ut COMMAND read_test)
set_property(TEST read_ut PROPERTY WILL_FAIL true)

