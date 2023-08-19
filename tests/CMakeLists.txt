# cmake_minimum_required(VERSION 3.15...3.23)

# project(CmakeConfigPackageTests LANGUAGES CXX)

# # ---- Test as standalone project the exported config package ----

# if(PROJECT_IS_TOP_LEVEL OR TEST_INSTALLED_VERSION)
# enable_testing()

# find_package(antumbra CONFIG REQUIRED) # for intro, project_options, ...

# if(NOT TARGET antumbra_options)
# message(FATAL_ERROR "Requiered config package not found!")
# return() # be strictly paranoid for Template Janitor github action! CK
# endif()
# endif()

# # ---- Dependencies ----

# include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)

# # Provide a simple smoke test to make sure that the CLI works and can display a --help message
# add_test(NAME cli.has_help COMMAND intro --help)

# # Provide a test to verify that the version being reported from the application
# # matches the version given to CMake. This will be important once you package
# # your program. Real world shows that this is the kind of simple mistake that is easy
# # to make, but also easy to test for.
# add_test(NAME cli.version_matches COMMAND intro --version)
# set_tests_properties(cli.version_matches PROPERTIES PASS_REGULAR_EXPRESSION "${PROJECT_VERSION}")

# add_executable(tests tests.cpp)
# target_link_libraries(
# tests
# PRIVATE antumbra::antumbra_warnings
# antumbra::antumbra_options
# antumbra::sample_library
# Catch2::Catch2WithMain)

# if(WIN32 AND BUILD_SHARED_LIBS)
# add_custom_command(
# TARGET tests
# PRE_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:tests> $<TARGET_FILE_DIR:tests>
# COMMAND_EXPAND_LISTS)
# endif()

# # automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# # to whatever you want, or use different for different binaries
# catch_discover_tests(
# tests
# TEST_PREFIX
# "unittests."
# REPORTER
# XML
# OUTPUT_DIR
# .
# OUTPUT_PREFIX
# "unittests."
# OUTPUT_SUFFIX
# .xml)

# # Add a file containing a set of constexpr tests
# add_executable(constexpr_tests constexpr_tests.cpp)
# target_link_libraries(
# constexpr_tests
# PRIVATE antumbra::antumbra_warnings
# antumbra::antumbra_options
# antumbra::sample_library
# Catch2::Catch2WithMain)

# catch_discover_tests(
# constexpr_tests
# TEST_PREFIX
# "constexpr."
# REPORTER
# XML
# OUTPUT_DIR
# .
# OUTPUT_PREFIX
# "constexpr."
# OUTPUT_SUFFIX
# .xml)

# # Disable the constexpr portion of the test, and build again this allows us to have an executable that we can debug when
# # things go wrong with the constexpr testing
# add_executable(relaxed_constexpr_tests constexpr_tests.cpp)
# target_link_libraries(
# relaxed_constexpr_tests
# PRIVATE antumbra::antumbra_warnings
# antumbra::antumbra_options
# antumbra::sample_library
# Catch2::Catch2WithMain)
# target_compile_definitions(relaxed_constexpr_tests PRIVATE -DCATCH_CONFIG_RUNTIME_STATIC_REQUIRE)

# catch_discover_tests(
# relaxed_constexpr_tests
# TEST_PREFIX
# "relaxed_constexpr."
# REPORTER
# XML
# OUTPUT_DIR
# .
# OUTPUT_PREFIX
# "relaxed_constexpr."
# OUTPUT_SUFFIX
# .xml)

project(tests)

file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

# source_group(TREE "${ENGINE_ROOT_DIR}/schema" FILES ${SCHEMA_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

copy_dxil(${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR})

configure_compile_options_no_werror(${PROJECT_NAME})

# include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    framework)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/doctest/doctest)

# link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
    doctest_with_main
    framework
)