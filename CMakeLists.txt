cmake_minimum_required(VERSION 3.28)
project(CameraTest)

set(CMAKE_CXX_STANDARD 20)

include_directories(CameraTest 3rdParty)

add_executable(CameraTest main.cpp)

