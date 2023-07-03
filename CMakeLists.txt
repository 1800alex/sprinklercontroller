cmake_minimum_required(VERSION 3.10)
project(spc)

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
        "${CMAKE_SOURCE_DIR}/src/main.cpp",
        "${CMAKE_SOURCE_DIR}/impl/sim/*.c"
        "${CMAKE_SOURCE_DIR}/impl/sim/*.cpp"
    )
    add_executable(spc ${COMMON_SOURCES} ${LINUX_SOURCES})

    # Generate compile_flags.txt for Linux development
    set(COMPILE_FLAGS "-DLINUX -std=c++17 -Wall -Wextra")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    file(WRITE ${CMAKE_BINARY_DIR}/compile_flags.txt ${COMPILE_FLAGS})

else()
    # Raspberry Pi Pico target
    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
    set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdlib")

    include_directories("${CMAKE_SOURCE_DIR}")

    # Add linker script for Pico
    add_link_options(-T${CMAKE_SOURCE_DIR}/pico_sdk/src/rp2_common/pico_base/include/pico/bootrom/pico_base.ld)

    # Include the Pico SDK
    include_directories(pico_sdk/src/rp2_common/pico_base/include)
    include_directories(pico_sdk/src/rp2_common/pico_runtime)

    # Add your Pico source files
    file(GLOB_RECURSE PICO_SOURCES
        "${CMAKE_SOURCE_DIR}/src/main.cpp"
    )
    add_executable(spc ${COMMON_SOURCES} ${PICO_SOURCES})

    # Generate compile_flags.txt for Pico development
    set(COMPILE_FLAGS "-std=c++11")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    file(WRITE ${CMAKE_BINARY_DIR}/compile_flags.txt ${COMPILE_FLAGS})
endif()

# Link external libraries for Linux target
if(LINUX_TARGET)
    target_link_libraries(spc ${CURSES_LIBRARIES} ${GTEST_LIBRARIES})

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

    # # Set up testing
    # enable_testing()
    # add_subdirectory(test)
endif()
