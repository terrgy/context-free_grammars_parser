project(all_tests)

include_directories(Grammar)
add_subdirectory(Grammar)

add_executable(all_tests all_tests.cpp
        Grammar/GrammarTestCase.cpp
        ${grammar-source-files}
)
target_link_libraries(
        all_tests gtest_main gtest gmock Threads::Threads cpr
)

setup_target_for_coverage(
        all_tests_coverage_tree "." all_tests all_tests_coverage
)

add_dependencies(coverage_report all_tests_coverage_tree)