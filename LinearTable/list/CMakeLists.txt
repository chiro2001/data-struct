cmake_minimum_required(VERSION 3.0.0)
project(radar VERSION 0.2)

#include(CTest)
enable_testing()

message(STATUS "${PROJECT_NAME} started building on ${CMAKE_SYSTEM_NAME}!")

# Include
set(include_dirs "${PROJECT_SOURCE_DIR}")
message(STATUS "Include dirs: ${include_dirs}")
include_directories(${include_dirs})

# Src

file(GLOB_RECURSE source_files "${PROJECT_SOURCE_DIR}/list.h" "${PROJECT_SOURCE_DIR}/list.c")
message(STATUS "Source files: ${source_files}")

add_executable(list_main ${PROJECT_SOURCE_DIR}/list_main.c ${source_files})
#target_link_libraries(serial-test glog::glog ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
