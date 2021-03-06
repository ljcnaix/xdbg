cmake_minimum_required (VERSION 3.0)
project (XDbg)

set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")

include_directories(ext/libelfin ext/linenoise include)

aux_source_directory(src XDBG)
add_executable(xdbg ${XDBG} ext/linenoise/linenoise.c)

add_executable(hello examples/hello.cpp)
set_target_properties(hello PROPERTIES COMPILE_FLAGS "-g -O0")

add_executable(variable examples/variable.cpp)
set_target_properties(variable PROPERTIES COMPILE_FLAGS "-gdwarf-2 -O0")

add_executable(unwinding examples/stack_unwinding.cpp)
set_target_properties(unwinding PROPERTIES COMPILE_FLAGS "-g -O0")

add_custom_target(
    libelfin
    COMMAND make
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/ext/libelfin
)

target_link_libraries(
    xdbg 
    ${PROJECT_SOURCE_DIR}/ext/libelfin/dwarf/libdwarf++.so
    ${PROJECT_SOURCE_DIR}/ext/libelfin/elf/libelf++.so
)

add_dependencies(xdbg libelfin)
