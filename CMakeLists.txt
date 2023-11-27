cmake_minimum_required(VERSION 3.8)
project(formal)

include(ExternalProject)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" ${CMAKE_SOURCE_DIR}/cmake)

set(EXTERNAL_INSTALL_LOCATION ${Testing_SOURCE_DIR}/3rdparty)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${formal_SOURCE_DIR}/bin)
SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage --coverage")
SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

SET(CPR_USE_SYSTEM_GTEST ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)

if (APPLE)

    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl@1.1" CACHE INTERNAL "")
    set(OPENSSL_LIBRARIES "/usr/local/opt/openssl@1.1/lib" CACHE INTERNAL "")
    FetchContent_Declare(
            cpr_repo
            GIT_REPOSITORY https://github.com/libcpr/cpr.git
    )
else()
    FetchContent_Declare(
            cpr_repo
            GIT_REPOSITORY https://github.com/libcpr/cpr.git
    )
endif()

FetchContent_MakeAvailable(cpr_repo googletest)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
include_directories(${CPR_INCLUDE_DIRS})

include(CodeCoverage)
add_custom_target(coverage_report)

function(run_coverage test_sources sources libraries coverage_target target_name coverage_name)
    message("INFO test sources ${test_sources}")
    set(test_name "${target_name}-test")
    message("INFO sources ${sources}")
    add_executable(${test_name} ${test_sources} ${sources})


    target_link_libraries(
            ${test_name} ${libraries} gtest_main gtest gmock Threads::Threads cpr curl
    )

    setup_target_for_coverage(
            ${coverage_target} ${target_name} ${test_name} ${coverage_name}
    )

    add_dependencies(coverage_report ${coverage_target})
endfunction()


set(SANITIZER "address")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(NOT SANITIZER)
        set(SANITIZER "address")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${SANITIZER}")
endif()

include_directories(include)
include_directories(include/Parsers)

add_subdirectory(src)
add_subdirectory(tests)