set(TARGET_NAME "grammar")
set(COVERAGE_NAME "grammar-coverage")
set(COVERAGE_TARGET "grammar-coverage-tree")
set(SOURCE_DIR_MAIN "${grammar_SOURCE_DIR}")
set(SOURCES ${grammar-source-files})
set(LIBRARIES ${grammar-libraries} regex)

message(INFO ${SOURCES})

set(TEST_SOURCES GrammarTestCase.h GrammarTestCase.cpp)
run_coverage(
        "${TEST_SOURCES}"
        "${SOURCES}"
        "${LIBRARIES}"
        "${COVERAGE_TARGET}"
        "${TARGET_NAME}"
        "${COVERAGE_NAME}"
)