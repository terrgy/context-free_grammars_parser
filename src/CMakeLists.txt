add_subdirectory(libs)

add_executable(main main.cpp)
target_link_libraries(main grammar earley-parser lr1-parser)