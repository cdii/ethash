cmake_minimum_required(VERSION 3.21)

project(ethash LANGUAGES C CXX)

#===============================================================================
# cmake configuration
#===============================================================================
set(CMAKE_CONFIGURATION_TYPES Debug Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

foreach(config ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${config} config)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endforeach()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#===============================================================================
# compile options
#===============================================================================
add_library(ethash_compile_options INTERFACE)
target_compile_features(ethash_compile_options INTERFACE c_std_11 cxx_std_17)
target_compile_options(ethash_compile_options
  INTERFACE
    $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall -Werror -Wextra -pedantic -pedantic-errors>
    $<$<COMPILE_LANG_AND_ID:C,MSVC>:/W4 /WX>
    $<$<COMPILE_LANG_AND_ID:CXX,Clang,GNU>:-Wall -Werror -Wextra -pedantic -pedantic-errors>
    $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/W4 /WX>)

#===============================================================================
# dependencies
#===============================================================================
add_library(ethash_dependencies INTERFACE)
# find_package(Threads REQUIRED)
# target_link_libraries(ethash_dependencies INTERFACE Threads::Threads)

#===============================================================================
add_library(ethash_include_directories INTERFACE)
target_include_directories(ethash_include_directories INTERFACE include lib)

#===============================================================================
add_library(${PROJECT_NAME}
  STATIC
    lib/ethash/bit_manipulation.h
    lib/ethash/builtins.h
    lib/ethash/endianness.hpp
    lib/ethash/ethash-internal.hpp
    lib/ethash/ethash.cpp
    lib/ethash/kiss99.hpp
    lib/ethash/managed.cpp
    lib/ethash/primes.c
    lib/ethash/primes.h
    lib/ethash/progpow.cpp
    lib/keccak/keccak.c
    lib/keccak/keccakf1600.c
    lib/keccak/keccakf800.c
    lib/support/attributes.h
    include/ethash/ethash.h
    include/ethash/ethash.hpp
    include/ethash/hash_types.h
    include/ethash/hash_types.hpp
    include/ethash/keccak.h
    include/ethash/keccak.hpp
    include/ethash/progpow.hpp
    include/ethash/version.h)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ethash_compile_options
    ethash_dependencies
    ethash_include_directories)

#===============================================================================
enable_testing()

option(INSTALL_GTEST "" OFF)
add_subdirectory(deps/googletest EXCLUDE_FROM_ALL)
set_property(TARGET gtest PROPERTY FOLDER deps)
set_property(TARGET gtest_main PROPERTY FOLDER deps)
include(GoogleTest)

add_executable(ethash_test
  tests/firopow_test_vectors.hpp
  tests/helpers.hpp
  tests/kawpow_test_vectors.hpp
  tests/progpow_test_vectors.hpp
  tests/test_bit_manipulation.cpp
  tests/test_cases.hpp
  tests/test_ethash.cpp
  tests/test_firopow.cpp
  tests/test_kawpow.cpp
  tests/test_keccak.cpp
  tests/test_kiss.cpp
  tests/test_managed.cpp
  tests/test_primes.cpp
  tests/test_progpow.cpp)
target_link_libraries(ethash_test
  PRIVATE
    ${PROJECT_NAME}
    ethash_compile_options
    ethash_dependencies
    ethash_include_directories
    gtest_main)
set_property(TARGET ethash_test PROPERTY FOLDER tests)
gtest_add_tests(TARGET ethash_test)
