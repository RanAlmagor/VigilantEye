# -----------------------------------------------------------------------------
# VigilantEye - Build Configuration
# Architecture: Modular C++17 (Core, HAL, Vision, Utils, UI)
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

# --- 1. VCPKG Integration (Environment Detection) ---
# Attempts to locate the vcpkg toolchain file automatically for local development.
# If not found, assumes the toolchain is passed via command line arguments.
set(LOCAL_VCPKG_PATH "C:/Users/hadma/vcpkg/scripts/buildsystems/vcpkg.cmake")

if(EXISTS ${LOCAL_VCPKG_PATH})
    set(CMAKE_TOOLCHAIN_FILE ${LOCAL_VCPKG_PATH})
    message(STATUS ">> Build System: Local vcpkg toolchain detected.")
else()
    message(STATUS ">> Build System: Standard environment (No local vcpkg forced).")
endif()

# --- 2. Project Metadata ---
project(VigilantEye VERSION 2.0 DESCRIPTION "Real-time UAV Tracking System")

# --- 3. C++ Standard Configuration ---
# Enforce C++17 standard for modern language features (smart pointers, filesystem).
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- 4. Dependencies ---
find_package(OpenCV REQUIRED)
message(STATUS ">> Dependency Check: OpenCV ${OpenCV_VERSION} Found.")

# --- 5. Source Discovery ---
# Recursively find all source (.cpp) and header (.h) files in the project structure.
# This eliminates the need to manually list every new file.
file(GLOB_RECURSE SOURCES 
    "src/*.cpp" 
    "include/*.h"
)

# --- 6. Include Directories ---
# Add include paths to allow direct imports (e.g., #include "WebcamSource.h")
include_directories(
    include
    include/Core
    include/HAL/Sensors
    include/HAL/Actuators
    include/Vision
    include/Utils
    include/UI
)

# --- 7. Target Definition ---
add_executable(VigilantEye ${SOURCES})

# --- 8. Linking ---
target_link_libraries(VigilantEye PRIVATE ${OpenCV_LIBS})

# --- 9. Build Summary ---
message(STATUS "-------------------------------------------------")
message(STATUS "VigilantEye Configuration Ready.")
message(STATUS "Target Architecture: HAL (Sensors/Actuators) | Core | Vision")
message(STATUS "-------------------------------------------------")