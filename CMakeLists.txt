cmake_minimum_required(VERSION 3.16)

project(MiniGameServer LANGUAGES CXX)

add_executable(MiniGameServer
    src/main.cpp
    src/TcpServer.cpp
    src/Session.cpp
    src/PacketCodec.cpp
)

target_compile_features(MiniGameServer PRIVATE cxx_std_17)
target_include_directories(MiniGameServer PRIVATE include)

if(WIN32)
    target_link_libraries(MiniGameServer PRIVATE ws2_32)
    target_compile_definitions(MiniGameServer PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

if(MSVC)
    target_compile_options(MiniGameServer PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(MiniGameServer PRIVATE -Wall -Wextra -Wpedantic)
endif()
