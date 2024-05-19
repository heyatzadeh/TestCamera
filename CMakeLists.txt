cmake_minimum_required(VERSION 3.28)
project(CameraTest)

set(CMAKE_CXX_STANDARD 20)

add_executable(CameraTest main.cpp)

file(GLOB lib_files "3rdParty/lib/${CMAKE_BUILD_TYPE}/*.lib")

target_include_directories(CameraTest PRIVATE "3rdParty/include")

target_link_libraries(CameraTest PRIVATE ${lib_files})

add_custom_command(TARGET CameraTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/bin/$<CONFIGURATION>
        ${CMAKE_BINARY_DIR}
)

