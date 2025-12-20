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
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Target
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

# Link
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

message(STATUS "VigilantEye Configuration Ready.")

# --- Visual Studio: run from project root (so relative paths work) ---
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# --- Copy runtime assets next to the .exe (portable run) ---
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/config"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/config"
)
