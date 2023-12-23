# This CMake file is intended to register project-wide objects.
# This allows for reuse between deployments, or other projects.

add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Components")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/RPi4/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Native/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Testbench/")
