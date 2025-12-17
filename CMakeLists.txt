# -----------------------------------------------------------------------------
# CMake Build Configuration for VigilantEye
# Target System: Embedded Linux / Windows
# Author: Ran Almagor
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

# vcpkg toolchain (must be set before the first configure)
set(LOCAL_VCPKG_PATH "C:/Users/hadma/vcpkg/scripts/buildsystems/vcpkg.cmake")

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND EXISTS "${LOCAL_VCPKG_PATH}")
    set(CMAKE_TOOLCHAIN_FILE "${LOCAL_VCPKG_PATH}" CACHE STRING "vcpkg toolchain file")
    message(STATUS "Build System: Local vcpkg toolchain detected (Ran's Environment).")
else()
    message(STATUS "Build System: Portable mode OR toolchain already set.")
endif()

project(VigilantEye
    VERSION 1.0
    DESCRIPTION "Real-time Embedded Vision Tracker"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

message(STATUS "Build System: VigilantEye Configuration Started")
message(STATUS "  - OpenCV Found: ${OpenCV_FOUND}")
message(STATUS "  - OpenCV Version: ${OpenCV_VERSION}")

add_executable(VigilantEye
    src/main.cpp

    app/App.cpp

    vision/Camera.cpp
    vision/ObjectTracker.cpp

    control/AutoFramingController.cpp

    telemetry/FpsCounter.cpp
    telemetry/Telemetry.cpp

    util/Timer.cpp
)

# Allow includes like: #include "vision/Camera.h"
target_include_directories(VigilantEye PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(VigilantEye PRIVATE ${OpenCV_LIBS})

message(STATUS "Build System: Ready. Press Play to launch.")
