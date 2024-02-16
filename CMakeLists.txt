cmake_minimum_required(VERSION 3.27)
project(CrossPlay)

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

target_link_libraries(CrossPlay Interfaces GameBoyColorEmulator)
