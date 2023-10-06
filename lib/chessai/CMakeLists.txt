cmake_minimum_required(VERSION 3.25.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ChessCore)

file(GLOB ai_sources
     src/*.hpp
     src/*.cpp
)
file(GLOB ai_headers
     include/*.hpp
)

file(GLOB ai_tests
     tests/*.cpp
)

add_library(chessai ${ai_headers} ${ai_sources})
target_link_libraries(chessai chesscore)
target_include_directories(chessai PUBLIC include/)

if(TESTING)

add_executable(chessaitests ${core_tests})
target_link_libraries(chessaitests chessai Catch2::Catch2WithMain)
add_test(NAME chessaitests COMMAND chesscoretests)

endif()
