project(all_tests)

include_directories(Grammar)
include_directories(Parsers/EarleyParser)
include_directories(Parsers/GeneralBaseParser)
include_directories(Parsers/LR1Parser)
add_subdirectory(Grammar)
add_subdirectory(Parsers)

add_executable(all_tests all_tests.cpp
        Grammar/GrammarTestCase.cpp
        Parsers/EarleyParser/EarleyParserTestCase.cpp
        Parsers/LR1Parser/LR1ParserTestCase.cpp
        Parsers/GeneralBaseParser/GeneralBaseParserTestCase.cpp
        ${grammar-source-files}
        ${earley-parser-source-files}
        ${lr1-parser-source-files}
)
target_link_libraries(
        all_tests gtest_main gtest gmock Threads::Threads cpr
)

setup_target_for_coverage(
        all_tests_coverage_tree "." all_tests all_tests_coverage
)

add_dependencies(coverage_report all_tests_coverage_tree)