cmake_minimum_required(VERSION 3.27)
project(CrossPlay)

# Replace with the actual path to SFML
set(SFML_DIR "/opt/homebrew/opt/sfml")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Find SFML package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(CMAKE_CXX_STANDARD 23)

# Add subdirectories for each project
add_subdirectory(Interfaces)
add_subdirectory(GameBoyColorEmulator)

# Add the main executable and link it with the projects
add_executable(CrossPlay main.cpp
        Hardware.h
        Clock.cpp
        Clock.h
        Definitions.h)

target_link_libraries(CrossPlay Interfaces GameBoyColorEmulator sfml-graphics sfml-window sfml-system)

target_include_directories(CrossPlay PRIVATE ${SFML_INCLUDE_DIR})
