cmake_minimum_required(VERSION 3.5)
project(tree_walking_interpreter VERSION 0.0.1 LANGUAGES CXX)

set(HEADERS
    include/varible_store.h
    include/control_flow_graph.h
    include/stack_machine.h
    include/json.h
    include/interpreter.h
    include/Function.h
    include/EnvScope.h
    include/VariableStore.h
)

set(SOURCES
    src/control_flow_graph.cpp
    src/stack_machine.cpp
    src/json.cpp
    src/interpreter.cpp
    src/Function.cpp
    src/EnvScope.cpp
    src/VariableStore.cpp
)

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC 
  "include" 
)

target_link_libraries(${PROJECT_NAME} PUBLIC parser )
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_subdirectory ("repl")
add_subdirectory ("tests/unit_tests")
