project(lr1-parser)

set(lr1-parser-source-files "${PROJECT_SOURCE_DIR}/LR1Parser.cpp" CACHE INTERNAL "")
set(lr1-parser-libraries grammar CACHE INTERNAL "")

add_library(lr1-parser SHARED ${PROJECT_SOURCE_DIR}/LR1Parser.cpp ${formal_SOURCE_DIR}/include/Parsers/LR1Parser.h)
target_link_libraries(lr1-parser ${lr1-parser-libraries})