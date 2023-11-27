set(TARGET_NAME "earley-parser")
set(COVERAGE_NAME "earley-parser-coverage")
set(COVERAGE_TARGET "earley-parser-coverage-tree")
set(SOURCE_DIR_MAIN "${earley-parser_SOURCE_DIR}")
set(SOURCES ${earley-parser-source-files})
set(LIBRARIES ${earley-parser-libraries} regex)

message(INFO ${SOURCES})

set(TEST_SOURCES GeneralEarleyParserTestCase.h EarleyParserTestCase.cpp)
run_coverage(
        "${TEST_SOURCES}"
        "${SOURCES}"
        "${LIBRARIES}"
        "${COVERAGE_TARGET}"
        "${TARGET_NAME}"
        "${COVERAGE_NAME}"
)