cmake_minimum_required(VERSION 3.27)
project(GameBoyColorEmulator)

# Add all your source files here
set(SOURCE_FILES
        GbCpu.h
        GbCpu.cpp
        Registers.h
        InterruptController.h
        InterruptController.cpp
        InterruptVectorTable.cpp
        InterruptVectorTable.h
        ISR.cpp
        ISR.h
        GbMemory.cpp
        GbMemory.h
        Consts.h
        Button.h
        MBC/MBC1.cpp
        MBC/MBC1.h
)

# Create the library
add_library(GameBoyColorEmulator STATIC ${SOURCE_FILES})

# Include directories
target_include_directories(GameBoyColorEmulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(GameBoyColorEmulator PUBLIC Interfaces)