cmake_minimum_required(VERSION 3.27)
project(Interfaces)

add_library(Interfaces INTERFACE
        IGpu.h
        ICpu.h
        IMemory.h
        IInputDevice.h
        IApu.h
        IStorage.h
        IMemoryBankController.h
)

target_include_directories(Interfaces INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
