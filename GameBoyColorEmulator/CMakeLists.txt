cmake_minimum_required(VERSION 3.27)
project(GameBoyColorEmulator)

# Replace with the actual path to SFML
set(SFML_DIR "/opt/homebrew/opt/sfml")

# Find SFML package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add all your source files here
set(SOURCE_FILES
        Cpu/GbCpu.h
        Cpu/GbCpu.cpp
        Registers.h
        Interrupts/InterruptController.h
        Interrupts/InterruptController.cpp
        Interrupts/InterruptVectorTable.cpp
        Interrupts/InterruptVectorTable.h
        Interrupts/ISR.cpp
        Interrupts/ISR.h
        Bus.cpp
        Bus.h
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
        PPU/GbPPU.cpp
        PPU/GbPPU.h
        Cpu/GbCpu.initXor.cpp
        Cpu/GbCpu.initRst.cpp
        Cpu/GbCpu.initLdb.cpp
        Cpu/GbCpu.initLda.cpp
        Cpu/GbCpu.initInc.cpp
        Cpu/GbCpu.initDec.cpp
        Cpu/GbCpu.initLdd.cpp
        Cpu/GbCpu.initLdh.cpp
        Cpu/GbCpu.initLdc.cpp
        Cpu/GbCpu.initLde.cpp
        Cpu/GbCpu.initLdl.cpp
        Cpu/GbCpu.initAdd.cpp
        Cpu/GbCpu.initAnd.cpp
        Cpu/GbCpu.initSub.cpp
        StopCpuException.h
        Cpu/GbCpu.initR.cpp
        Cpu/GbCpu.initJ.cpp
        Cpu/GbCpu.initC.cpp
        Cpu/GbCpu.initPushPop.cpp
        Cpu/PrefixCb/GbCpu.PrefixCb.InitR.cpp
        Cpu/PrefixCb/GbCpu.PrefixCb.InitS.cpp
        Cpu/PrefixCb/GbCpu.PrefixCb.InitB.cpp
        PPU/LcdControl.h
        Cartridges/Cartridge.cpp
        Cartridges/Cartridge.h
        Cartridges/CartridgeHeader.h
        Memory/Memory.cpp
        Memory/Bank.cpp
        Timer.cpp
        Timer.h
        Timer.h
)

# Create the library
add_library(GameBoyColorEmulator STATIC ${SOURCE_FILES})

# Include directories
target_include_directories(GameBoyColorEmulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${SFML_INCLUDE_DIR})

target_link_libraries(GameBoyColorEmulator PUBLIC Interfaces sfml-graphics sfml-window sfml-system)
