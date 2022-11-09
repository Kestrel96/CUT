cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)
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

## tests
list(FILTER SRC EXCLUDE REGEX "main.c")
message(STATUS ${SRC})
add_executable(read_test "./test/file_read_test.c" ${SRC})
target_compile_definitions(read_test PRIVATE STAT_PATH="../test/test_proc.txt")

add_test(NAME read_ut COMMAND read_test)



add_executable(watchdog_test  "./test/watchdog_test.c" ${SRC})

add_test(NAME watchdog_ut COMMAND watchdog_test)
set_property(TEST watchdog_ut PROPERTY WILL_FAIL true)
