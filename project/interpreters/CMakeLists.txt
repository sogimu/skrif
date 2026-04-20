cmake_minimum_required(VERSION 3.6)
project(interpreters LANGUAGES CXX)

add_subdirectory ("tree-walking-interpreter")
add_subdirectory ("bytecode-based-vm-interpreter")
