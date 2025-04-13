add_executable(example main.cpp)
target_link_libraries(example PRIVATE sqlbuilder)
target_include_directories(example PRIVATE .)
