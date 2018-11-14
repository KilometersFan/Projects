cmake_minimum_required(VERSION 3.1)

message("########################################################")
message("                 CS104 HW7 API Check")
message("########################################################")

project(CS104-HW7-API-Check C CXX)

# pick up include files
# ----------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/test_cmake_lib)

include(Policies NO_POLICY_SCOPE)
include(Utils)
include(Shorthand)
include(LibraryUtils)
include(104CompileFlags)

# find user code dir
# ----------------------------------------------------------

if(NOT DEFINED HW7_DIR)

	file(GLOB PARENT_DIR_CPP_FILES "${CMAKE_BINARY_DIR}/../*.cpp")

	if("${PARENT_DIR_CPP_FILES}" STREQUAL "")
		message(FATAL_ERROR "Cannot find source files in parent directory.  Either move this test directory to your hw7 dir, or run CMake with the \"-DHW7_DIR=<path to hw7>\" argument to use a custom location")
	else()
		get_filename_component(PARENT_DIR_REALPATH "${CMAKE_BINARY_DIR}/.." REALPATH)
		message(STATUS "HW7_DIR autoset to ${PARENT_DIR_REALPATH}")
		set(HW7_DIR ${PARENT_DIR_REALPATH} CACHE PATH "Path to hw7 assignment directory")
	endif()
endif()

# make sure HW7_DIR is an absolute path
get_filename_component(HW7_DIR ${HW7_DIR} REALPATH)

# Build checker code
# ----------------------------------------------------------
include_directories(${HW7_DIR})

set(ALL_CHECKERS bst_checker iterator_checker node_checker rotate_checker avl_checker avl_node_checker)

foreach(CHECKER ${ALL_CHECKERS})
	add_executable(${CHECKER} ${CHECKER}.cpp)

	# print a success message after each checker builds
	add_custom_command(TARGET ${CHECKER}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan --bold "${CHECKER} API Check PASS")
endforeach()

# make sure that nobody thinks these executables are supposed to do anything
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/compile_test_results_dont_run_these)
set_property(TARGET ${ALL_CHECKERS} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/compile_test_results_dont_run_these)

