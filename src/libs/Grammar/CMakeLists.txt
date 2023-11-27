project(grammar)

set(grammar-source-files "${PROJECT_SOURCE_DIR}/Grammar.cpp" CACHE INTERNAL "")

add_library(grammar SHARED ${PROJECT_SOURCE_DIR}/Grammar.cpp ${formal_SOURCE_DIR}/include/Grammar.h)