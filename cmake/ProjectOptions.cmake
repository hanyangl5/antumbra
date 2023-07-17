include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

function(configure_compile_options proj_name)
    set(CMAKE_CXX_STANDARD 17)
    target_compile_options(${PROJECT_NAME} PRIVATE -DNOMINMAX)
    if(WIN32)
        if(MSVC)
            message("using MSVC under windows")

            target_compile_options(${proj_name} PRIVATE /std:c++17)
            target_compile_options(${proj_name} PRIVATE /MP /permissive /w14640 /W4 /WX /external:anglebrackets /external:W0 /GR-)

        else()
            message("using GCC/Clang under windows")
            target_compile_options(${proj_name} PRIVATE -std=c++17)
            target_compile_options(${proj_name} PRIVATE -Wall -Wextra -Werror -Wshadow -Wnon-virtual-dtor -pedantic -fno-rtti)
        endif()
    elseif(LINUX)
        message("using GCC/Clang under LINUX")
        target_compile_options(${proj_name} PRIVATE -std=c++17)
        target_compile_options(${proj_name} PRIVATE -Wall -Wextra -Werror -Wshadow -Wnon-virtual-dtor -pedantic -fno-rtti)
    endif()

    # disable exceptions
    # MSVC string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) # disable exception
    # GCC/Clang  -fno-exceptions
endfunction()

function(configure_compile_options_no_werror proj_name)
    set(CMAKE_CXX_STANDARD 17)
    target_compile_options(${PROJECT_NAME} PRIVATE -DNOMINMAX)
    if(WIN32)
        if(MSVC)
            message("using MSVC under windows")

            target_compile_options(${proj_name} PRIVATE /std:c++17)
            target_compile_options(${proj_name} PRIVATE /MP /permissive /w14640 /W4 /external:anglebrackets /external:W0 /GR-)

        else()
            message("using GCC/Clang under windows")
            target_compile_options(${proj_name} PRIVATE -std=c++17)
            target_compile_options(${proj_name} PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic Weverything -fno-rtti)
        endif()
    elseif(LINUX)
        message("using GCC/Clang under LINUX")
        target_compile_options(${proj_name} PRIVATE -std=c++17)
        target_compile_options(${proj_name} PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic Weverything -fno-rtti)
    endif()

    # disable exceptions
    # MSVC string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) # disable exception
    # GCC/Clang  -fno-exceptions
endfunction()

macro(configure_antumbra)
    if(WIN32)
        if(MSVC)
            message("using MSVC under windows")
            add_compile_options(/MP /permissive /w14640 /WX /external:anglebrackets /external:W0 /GR- -D_HAS_EXCEPTIONS=0 -D_STATIC_CPPLIB)
            string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) # disable exception
        else()
            message("using GCC/Clang under windows")
            target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wshadow -Wnon-virtual-dtor -pedantic -fno-exceptions -fno-rtti)
        endif()
    elseif(LINUX)
        message("using GCC/Clang under LINUX")
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wshadow -Wnon-virtual-dtor -pedantic -fno-exceptions -fno-rtti)
    endif()
endmacro()

macro(antumbra_supports_sanitizers)
    if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
        set(SUPPORTS_UBSAN ON)
    else()
        set(SUPPORTS_UBSAN OFF)
    endif()

    if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
        set(SUPPORTS_ASAN OFF)
    else()
        set(SUPPORTS_ASAN ON)
    endif()
endmacro()

macro(antumbra_setup_options)
    option(antumbra_ENABLE_HARDENING "Enable hardening" ON)
    option(antumbra_ENABLE_COVERAGE "Enable coverage reporting" OFF)
    cmake_dependent_option(
        antumbra_ENABLE_GLOBAL_HARDENING
        "Attempt to push hardening options to built dependencies"
        ON
        antumbra_ENABLE_HARDENING
        OFF)

    antumbra_supports_sanitizers()

    if(NOT PROJECT_IS_TOP_LEVEL)
        option(antumbra_ENABLE_IPO "Enable IPO/LTO" OFF)
        option(antumbra_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
        option(antumbra_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
        option(antumbra_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
        option(antumbra_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
        option(antumbra_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
        option(antumbra_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
        option(antumbra_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
        option(antumbra_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
        option(antumbra_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
        option(antumbra_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
        option(antumbra_ENABLE_PCH "Enable precompiled headers" OFF)
        option(antumbra_ENABLE_CACHE "Enable ccache" OFF)
    else()
        option(antumbra_ENABLE_IPO "Enable IPO/LTO" ON)
        option(antumbra_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
        option(antumbra_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
        option(antumbra_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
        option(antumbra_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
        option(antumbra_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
        option(antumbra_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
        option(antumbra_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
        option(antumbra_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
        option(antumbra_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
        option(antumbra_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
        option(antumbra_ENABLE_PCH "Enable precompiled headers" OFF)
        option(antumbra_ENABLE_CACHE "Enable ccache" OFF)
    endif()

    if(NOT PROJECT_IS_TOP_LEVEL)
        mark_as_advanced(
            antumbra_ENABLE_IPO
            antumbra_WARNINGS_AS_ERRORS
            antumbra_ENABLE_USER_LINKER
            antumbra_ENABLE_SANITIZER_ADDRESS
            antumbra_ENABLE_SANITIZER_LEAK
            antumbra_ENABLE_SANITIZER_UNDEFINED
            antumbra_ENABLE_SANITIZER_THREAD
            antumbra_ENABLE_SANITIZER_MEMORY
            antumbra_ENABLE_UNITY_BUILD
            antumbra_ENABLE_CLANG_TIDY
            antumbra_ENABLE_CPPCHECK
            antumbra_ENABLE_COVERAGE
            antumbra_ENABLE_PCH
            antumbra_ENABLE_CACHE)
    endif()

    antumbra_check_libfuzzer_support(LIBFUZZER_SUPPORTED)

    if(LIBFUZZER_SUPPORTED AND(antumbra_ENABLE_SANITIZER_ADDRESS OR antumbra_ENABLE_SANITIZER_THREAD OR antumbra_ENABLE_SANITIZER_UNDEFINED))
        set(DEFAULT_FUZZER ON)
    else()
        set(DEFAULT_FUZZER OFF)
    endif()

    option(antumbra_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})
endmacro()

macro(antumbra_global_options)
    if(antumbra_ENABLE_IPO)
        include(cmake/InterproceduralOptimization.cmake)
        antumbra_enable_ipo()
    endif()

    antumbra_supports_sanitizers()

    if(antumbra_ENABLE_HARDENING AND antumbra_ENABLE_GLOBAL_HARDENING)
        include(cmake/Hardening.cmake)

        if(NOT SUPPORTS_UBSAN
            OR antumbra_ENABLE_SANITIZER_UNDEFINED
            OR antumbra_ENABLE_SANITIZER_ADDRESS
            OR antumbra_ENABLE_SANITIZER_THREAD
            OR antumbra_ENABLE_SANITIZER_LEAK)
            set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
        else()
            set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
        endif()

        message("${antumbra_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${antumbra_ENABLE_SANITIZER_UNDEFINED}")
        antumbra_enable_hardening(antumbra_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
    endif()
endmacro()

macro(antumbra_local_options)
    if(PROJECT_IS_TOP_LEVEL)
        include(cmake/StandardProjectSettings.cmake)
    endif()

    add_library(antumbra_warnings INTERFACE)
    add_library(antumbra_options INTERFACE)

    include(cmake/CompilerWarnings.cmake)
    antumbra_set_project_warnings(
        antumbra_warnings
        ${antumbra_WARNINGS_AS_ERRORS}
        ""
        ""
        ""
        "")

    if(antumbra_ENABLE_USER_LINKER)
        include(cmake/Linker.cmake)
        configure_linker(antumbra_options)
    endif()

    include(cmake/Sanitizers.cmake)
    antumbra_enable_sanitizers(
        antumbra_options
        ${antumbra_ENABLE_SANITIZER_ADDRESS}
        ${antumbra_ENABLE_SANITIZER_LEAK}
        ${antumbra_ENABLE_SANITIZER_UNDEFINED}
        ${antumbra_ENABLE_SANITIZER_THREAD}
        ${antumbra_ENABLE_SANITIZER_MEMORY})

    set_target_properties(antumbra_options PROPERTIES UNITY_BUILD ${antumbra_ENABLE_UNITY_BUILD})

    if(antumbra_ENABLE_PCH)
        target_precompile_headers(
            antumbra_options
            INTERFACE
            <vector>
            <string>
            <utility>)
    endif()

    # if(antumbra_ENABLE_CACHE)
    # include(cmake/Cache.cmake)
    # antumbra_enable_cache()
    # endif()
    include(cmake/StaticAnalyzers.cmake)

    if(antumbra_ENABLE_CLANG_TIDY)
        antumbra_enable_clang_tidy(antumbra_options ${antumbra_WARNINGS_AS_ERRORS})
    endif()

    if(antumbra_ENABLE_CPPCHECK)
        antumbra_enable_cppcheck(${antumbra_WARNINGS_AS_ERRORS} "" # override cppcheck options
        )
    endif()

    if(antumbra_ENABLE_COVERAGE)
        include(cmake/Tests.cmake)
        antumbra_enable_coverage(antumbra_options)
    endif()

    if(antumbra_WARNINGS_AS_ERRORS)
        check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)

        if(LINKER_FATAL_WARNINGS)
            # This is not working consistently, so disabling for now
            # target_link_options(antumbra_options INTERFACE -Wl,--fatal-warnings)
        endif()
    endif()

    if(antumbra_ENABLE_HARDENING AND NOT antumbra_ENABLE_GLOBAL_HARDENING)
        include(cmake/Hardening.cmake)

        if(NOT SUPPORTS_UBSAN
            OR antumbra_ENABLE_SANITIZER_UNDEFINED
            OR antumbra_ENABLE_SANITIZER_ADDRESS
            OR antumbra_ENABLE_SANITIZER_THREAD
            OR antumbra_ENABLE_SANITIZER_LEAK)
            set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
        else()
            set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
        endif()

        antumbra_enable_hardening(antumbra_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
    endif()
endmacro()
