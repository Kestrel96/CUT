cmake_minimum_required (VERSION 3.6)

project(CUT)

set(INC_DIR ./include)
set(SRC_DIR ./src)

aux_source_directory(${SRC_DIR} SRC)
include_directories(${INC_DIR})

add_executable(cut ${SRC})