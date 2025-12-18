# ---------------------------------------------------------------------------
# VigilantEye - Build Configuration (Portable)
# ---------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

project(VigilantEye VERSION 2.0 DESCRIPTION "Real-time UAV Tracking System" LANGUAGES CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenCV 
find_package(OpenCV REQUIRED)
message(STATUS ">> Dependency Check: OpenCV ${OpenCV_VERSION} Found.")

# --- Sources ---
set(VEYE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

file(GLOB_RECURSE VEYE_EXTRA_CPP CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/app/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vision/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/control/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/telemetry/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/hal/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp"   
)

list(APPEND VEYE_SOURCES ${VEYE_EXTRA_CPP})

# Target
add_executable(${PROJECT_NAME} ${VEYE_SOURCES})


target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}            
)

# Link
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${VEYE_SOURCES})

message(STATUS "-------------------------------------------------")
message(STATUS "VigilantEye Configuration Ready.")
message(STATUS "Target Architecture: HAL | Core | Vision | Utils | UI")
message(STATUS "-------------------------------------------------")
