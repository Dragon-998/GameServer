cmake_minimum_required(VERSION 3.16)

project(MiniGameServer LANGUAGES CXX)

if(WIN32)
    set(MINIGAMESERVER_SQLITE_TARGET winsqlite3)
else()
    find_package(SQLite3 3.24 REQUIRED)
    set(MINIGAMESERVER_SQLITE_TARGET SQLite::SQLite3)
endif()

add_executable(MiniGameServer
    src/main.cpp
    src/TcpServer.cpp
    src/Session.cpp
    src/PacketCodec.cpp
    src/Player.cpp
    src/PlayerManager.cpp
    src/PlayerStorage.cpp
    src/Dispatcher.cpp
    src/LoginHandler.cpp
    src/MoveHandler.cpp
    src/FightHandler.cpp
)

target_compile_features(MiniGameServer PRIVATE cxx_std_17)
target_include_directories(MiniGameServer PRIVATE include)

if(WIN32)
    target_link_libraries(MiniGameServer PRIVATE ws2_32 ${MINIGAMESERVER_SQLITE_TARGET})
    target_compile_definitions(MiniGameServer PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
else()
    target_link_libraries(MiniGameServer PRIVATE ${MINIGAMESERVER_SQLITE_TARGET})
endif()

if(MSVC)
    target_compile_options(MiniGameServer PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(MiniGameServer PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()

add_executable(DispatcherTest
    tests/DispatcherTest.cpp
    src/Dispatcher.cpp
    src/LoginHandler.cpp
    src/MoveHandler.cpp
    src/FightHandler.cpp
    src/PacketCodec.cpp
    src/Player.cpp
    src/PlayerManager.cpp
    src/PlayerStorage.cpp
    src/Session.cpp
)
target_compile_features(DispatcherTest PRIVATE cxx_std_17)
target_include_directories(DispatcherTest PRIVATE include)
if(WIN32)
    target_link_libraries(DispatcherTest PRIVATE ws2_32 ${MINIGAMESERVER_SQLITE_TARGET})
else()
    target_link_libraries(DispatcherTest PRIVATE ${MINIGAMESERVER_SQLITE_TARGET})
endif()
add_test(NAME DispatcherTest COMMAND DispatcherTest)

add_executable(PlayerStorageTest
    tests/PlayerStorageTest.cpp
    src/Player.cpp
    src/PlayerStorage.cpp
)
target_compile_features(PlayerStorageTest PRIVATE cxx_std_17)
target_include_directories(PlayerStorageTest PRIVATE include)
target_link_libraries(PlayerStorageTest PRIVATE ${MINIGAMESERVER_SQLITE_TARGET})
add_test(NAME PlayerStorageTest COMMAND PlayerStorageTest)
