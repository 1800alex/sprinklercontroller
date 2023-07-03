cmake_minimum_required(VERSION 3.10)
project(spc C CXX ASM)

# Check the target platform
if(DEFINED ENV{RASPBERRY_PI_PICO})
    # Raspberry Pi Pico target
    message("Configuring for Raspberry Pi Pico")
    set(PI_TARGET ON)
else()
    # Linux target
    message("Configuring for Linux")
    set(LINUX_TARGET ON)
endif()

# Add your common source files for both targets
file(GLOB_RECURSE COMMON_SOURCES
    "${CMAKE_SOURCE_DIR}/lib/*.c"
    "${CMAKE_SOURCE_DIR}/lib/*.cpp"
)

# add_library(spclib ${COMMON_SOURCES})

# Set compiler flags and options for each target
if(LINUX_TARGET)
    # Linux target
    add_compile_definitions(LINUX)
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    set(CMAKE_CXX_STANDARD 17)

    include_directories("${CMAKE_SOURCE_DIR}")

    # Add external libraries for Linux
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIRS})

    # Add Google Test
    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    # Add your Pico source files
    file(GLOB_RECURSE LINUX_SOURCES
        "${CMAKE_SOURCE_DIR}/src/main.cpp"
        "${CMAKE_SOURCE_DIR}/impl/sim/*.c"
        "${CMAKE_SOURCE_DIR}/impl/sim/*.cpp"
    )
    add_executable(${PROJECT_NAME} ${COMMON_SOURCES} ${LINUX_SOURCES})

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # # Generate compile_flags.txt for Linux development
    # set(COMPILE_FLAGS "-DLINUX -std=c++17 -Wall -Wextra")
    # file(WRITE ${CMAKE_BINARY_DIR}/compile_flags.txt ${COMPILE_FLAGS})

    target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES} ${GTEST_LIBRARIES})

    # Locate the Google Test package
    find_package(GTest REQUIRED)

    # Include directories
    include_directories(${GTEST_INCLUDE_DIRS})

    file(GLOB_RECURSE TEST_SOURCES
        "${CMAKE_SOURCE_DIR}/test/*.c"
        "${CMAKE_SOURCE_DIR}/test/*.cpp"
    )

    # Add the test executable
    add_executable(spctest ${COMMON_SOURCES} ${TEST_SOURCES})

    # Link against the Google Test library and your project library
    target_link_libraries(spctest ${GTEST_BOTH_LIBRARIES})

    # Run the tests
    enable_testing()
    add_test(NAME spctest COMMAND spctest)
endif()

if(PI_TARGET)
    # Include build functions from Pico SDK
    include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

    # Raspberry Pi Pico target
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 17)

    if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
        message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
    endif()

    # Initialize the SDK
    pico_sdk_init()

    include_directories("${CMAKE_SOURCE_DIR}")

    # Add your Pico source files
    file(GLOB_RECURSE PICO_SOURCES
        "${CMAKE_SOURCE_DIR}/src/main.cpp"
    )
    add_executable(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/pico/main.c")
    # add_executable(${PROJECT_NAME} ${COMMON_SOURCES} ${PICO_SOURCES})

    # pull in common dependencies
    target_link_libraries(${PROJECT_NAME} pico_stdlib)

    # Enable usb output, disable uart output
    pico_enable_stdio_usb(${PROJECT_NAME} 1)
    pico_enable_stdio_uart(${PROJECT_NAME} 0)

    # create map/bin/hex file etc.
    pico_add_extra_outputs(${PROJECT_NAME})

    add_compile_options(-Wall
            -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
            -Wno-unused-function # we have some for the docs that aren't called
            )
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-Wno-maybe-uninitialized)
    endif()

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # Generate compile_flags.txt for Pico development
    # set(COMPILE_FLAGS "-std=c++11")
    # file(WRITE ${CMAKE_BINARY_DIR}/compile_flags.txt ${COMPILE_FLAGS})
endif()
