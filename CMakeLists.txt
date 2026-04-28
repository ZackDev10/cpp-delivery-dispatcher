cmake_minimum_required(VERSION 3.16)
project(SimulateurLivraison CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simulateur
    main.cpp
    src/SystemeLivraison.cpp
)

# This lets ALL files use #include "SystemeLivraison.h" without any path prefix
target_include_directories(simulateur PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
