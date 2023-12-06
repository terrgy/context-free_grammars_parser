project(earley-parser)

set(earley-parser-source-files "${PROJECT_SOURCE_DIR}/EarleyParser.cpp" CACHE INTERNAL "")
set(earley-parser-libraries grammar CACHE INTERNAL "")

add_library(earley-parser SHARED ${PROJECT_SOURCE_DIR}/EarleyParser.cpp ${formal_SOURCE_DIR}/include/Parsers/EarleyParser.h)
target_link_libraries(earley-parser ${earley-parser-libraries})