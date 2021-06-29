cmake_minimum_required(VERSION 3.16)

project(learn-asio)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# asio/1.18.2
add_library(learn-asio INTERFACE)
target_include_directories(learn-asio INTERFACE ${CMAKE_SOURCE_DIR}/include)
if(UNIX)
    target_link_libraries(learn-asio INTERFACE pthread)
endif()

add_library(ocfbnj::learn-asio ALIAS learn-asio)

add_executable(main main.cpp main.cpp)
target_link_libraries(main PRIVATE ocfbnj::learn-asio)

if(MSVC)
    target_compile_definitions(main PRIVATE _WIN32_WINNT=0x0601)
endif()
