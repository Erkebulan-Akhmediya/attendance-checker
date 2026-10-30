cmake_minimum_required(VERSION 3.21)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(attendance-checker)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(main ${SOURCES})
target_compile_options(main PRIVATE -Wall -Wextra -Werror -g)

find_package(CURL)
target_link_libraries(main PRIVATE CURL::libcurl)

find_package(SQLite3)
add_library(SQLite3::SQLite3 ALIAS SQLite::SQLite3)
target_link_libraries(main PRIVATE SQLite3::SQLite3)

add_library(nlohmann_json INTERFACE)
# SYSTEM keeps warnings from the third-party header out of -Werror
target_include_directories(nlohmann_json SYSTEM INTERFACE ${PROJECT_SOURCE_DIR}/../extern)
target_link_libraries(main PRIVATE nlohmann_json)
