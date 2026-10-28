cmake_minimum_required(VERSION 3.2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(attendance-checker)
file(GLOB SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(main ${SOURCES})

find_package(CURL)
target_link_libraries(main PRIVATE CURL::libcurl)

add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${PROJECT_SOURCE_DIR}/../extern)
target_link_libraries(main PRIVATE nlohmann_json)
