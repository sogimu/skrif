cmake_minimum_required(VERSION 3.6)
project(bytecode_vm_interpreter_unit_tests VERSION 0.0.1 LANGUAGES CXX)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Enabling AddressSanitizer")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

add_executable(${PROJECT_NAME}
    src/arithmetic_expression/arithmetic_test.cpp
    src/variable_assignment/variable_assignment_test.cpp
    src/if_statement/if_statement_test.cpp
    src/while_statement/while_statement_test.cpp
    src/function_call/function_call_test.cpp
    src/member_expression/member_expression_test.cpp
    src/complex_scenarios/complex_scenarios_test.cpp
    src/main.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC bytecode_vm_interpreter GTest::gtest_main)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_test(NAME bytecode_vm_interpreter_unit_tests COMMAND ${PROJECT_NAME})
