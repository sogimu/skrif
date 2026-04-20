cmake_minimum_required(VERSION 3.5)
project(bytecode_vm_interpreter VERSION 0.0.1 LANGUAGES CXX)

set(HEADERS
    include/opcode.h
    include/instruction.h
    include/chunk.h
    include/call_frame.h
    include/json.h
    include/Function.h
    include/EnvScope.h
    include/copy_or_ref.h
    include/compiler.h
    include/vm.h
    include/interpreter.h
)

set(SOURCES
    src/json.cpp
    src/Function.cpp
    src/EnvScope.cpp
    src/chunk.cpp
    src/compiler.cpp
    src/vm.cpp
    src/interpreter.cpp
)

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_link_libraries(${PROJECT_NAME} PUBLIC parser)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_subdirectory("repl")
add_subdirectory("tests/unit_tests")
