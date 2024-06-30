cmake_minimum_required(VERSION 3.28)
project(CameraTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

list(APPEND CMAKE_PREFIX_PATH "C:/Qt/msvc2022_86")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

qt_add_executable(CameraTest
        main.cpp
        mainwindow.cpp
        mainwindow.h
        Tester.cpp
        Tester.h
        ImageData.h)

file(GLOB lib_files "3rdParty/lib/${CMAKE_BUILD_TYPE}/*.lib")

target_include_directories(CameraTest PRIVATE "3rdParty/include")

target_link_libraries(CameraTest PRIVATE ${lib_files} Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia)

set_target_properties(CameraTest PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)

add_custom_command(TARGET CameraTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/bin/$<CONFIGURATION>
        ${CMAKE_BINARY_DIR}
)

