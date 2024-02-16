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
        MBC/NoMBC.cpp
        MBC/NoMBC.h
        MBC/mbc2.cpp
        MBC/mbc2.h
        MBC/MBC3.cpp
        MBC/MBC3.h
        MBC/MBC4.cpp
        MBC/MBC4.h
        MBC/MBC5.cpp
        MBC/MBC5.h
        MBC/MBC6.cpp
        MBC/MBC6.h
        MBC/MBC7.cpp
        MBC/MBC7.h
)

# Create the library
add_library(GameBoyColorEmulator STATIC ${SOURCE_FILES})

# Include directories
target_include_directories(GameBoyColorEmulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(GameBoyColorEmulator PUBLIC Interfaces)