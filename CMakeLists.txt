cmake_minimum_required(VERSION 3.30)

project(SyncUp)
set(CMAKE_CXX_STANDARD 20)

add_executable(syncup Sources/main.cpp)
install(TARGETS syncup)
