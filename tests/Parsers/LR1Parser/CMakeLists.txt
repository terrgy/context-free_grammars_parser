set(TARGET_NAME "lr1-parser")
set(COVERAGE_NAME "lr1-parser-coverage")
set(COVERAGE_TARGET "lr1-parser-coverage-tree")
set(SOURCE_DIR_MAIN "${lr1-parser_SOURCE_DIR}")
set(SOURCES ${lr1-parser-source-files})
set(LIBRARIES ${lr1-parser-libraries} regex)

message(INFO ${SOURCES})

set(TEST_SOURCES LR1ParserTestCase.h LR1ParserTestCase.cpp)
run_coverage(
        "${TEST_SOURCES}"
        "${SOURCES}"
        "${LIBRARIES}"
        "${COVERAGE_TARGET}"
        "${TARGET_NAME}"
        "${COVERAGE_NAME}"
)