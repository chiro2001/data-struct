cmake_minimum_required(VERSION 3.0.0)
project(ex5 VERSION 0.2)

#include(CTest)
enable_testing()

#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

message(STATUS "${PROJECT_NAME} started building on ${CMAKE_SYSTEM_NAME}!")


file(GLOB_RECURSE resources "${PROJECT_SOURCE_DIR}/resource/*")
message(STATUS "Copying resources file(s): ${resources}")
file(COPY ${resources} DESTINATION ${CMAKE_BINARY_DIR}/ FOLLOW_SYMLINK_CHAIN)

# Include
set(include_dirs "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/chilib")
message(STATUS "Include dirs: ${include_dirs}")
include_directories(${include_dirs})

# Src

file(GLOB_RECURSE source_files
        "${PROJECT_SOURCE_DIR}/chilib/*.hpp")
message(STATUS "Source files: ${source_files}")

add_executable(task1 ${PROJECT_SOURCE_DIR}/task1.c ${source_files})
add_executable(task2 ${PROJECT_SOURCE_DIR}/task2.cpp ${source_files})
add_executable(task3 ${PROJECT_SOURCE_DIR}/task3.cpp ${source_files})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
