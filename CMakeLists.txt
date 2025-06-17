cmake_minimum_required(VERSION 3.10)
project(ChessDemoAI CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the source files
set(SOURCES
    src/main.cpp
    src/chess_engine.cpp
    src/evaluation.cpp
    src/alpha_beta.cpp
    src/minimax.cpp
    src/web_server.cpp
    src/transposition_table.cpp
    src/ab_tt.cpp
)

# Set the include directories
# Assumes chess.hpp and other chess-library headers are in a directory named 'chess-library/include'
# located at the same level as 'src' and 'include' directories of this project.
# Adjust the path "../chess-library/include" if chess.hpp is located elsewhere.
include_directories(
    include
    ../chess-library/include # Path to chess.hpp and related headers
)

add_executable(chess_ai_demo ${SOURCES})

# Add common compiler warnings
if(MSVC)
    target_compile_options(chess_ai_demo PRIVATE /W4)
else()
    target_compile_options(chess_ai_demo PRIVATE -Wall -Wextra -pedantic)
endif()

# If chess-library is not header-only and requires linking a library:
# Предположим, что библиотека называется 'libchess.a' или 'chess.lib'
# и находится в '../chess-library/lib'
# target_link_directories(chess_ai_demo PRIVATE ../chess-library/lib)
# target_link_libraries(chess_ai_demo PRIVATE chess) # Name of the library without 'lib' prefix or extension

# Instructions for the user:
# 1. Make sure you have chess-library installed or its headers available.
# 2. If chess.hpp is not in a directory like '../chess-library/include' relative to this CMakeLists.txt,
#    update the include_directories() path accordingly.
# 3. If chess-library requires linking a compiled library, uncomment and adjust
#    target_link_directories() and target_link_libraries().
# 4. To build:
#    mkdir build
#    cd build
#    cmake ..
#    cmake --build . 
#    (or use 'make' if cmake generates Makefiles)
# 5. Run the executable: ./chess_ai_demo (or build\Debug\chess_ai_demo.exe on Windows) 

if(WIN32)
    target_link_libraries(chess_ai_demo PRIVATE ws2_32 wsock32)
endif() 