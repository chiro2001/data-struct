cmake_minimum_required(VERSION 3.0.0)
project(btree VERSION 0.2)

#include(CTest)
enable_testing()

message(STATUS "${PROJECT_NAME} started building on ${CMAKE_SYSTEM_NAME}!")

# Include
set(include_dirs "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/chilib")
message(STATUS "Include dirs: ${include_dirs}")
include_directories(${include_dirs})

# Src

file(GLOB_RECURSE source_files
        "${PROJECT_SOURCE_DIR}/chilib/*.hpp")
message(STATUS "Source files: ${source_files}")

add_executable(main ${PROJECT_SOURCE_DIR}/main.cpp ${source_files})
add_executable(btree_test ${PROJECT_SOURCE_DIR}/test.cpp ${source_files})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
