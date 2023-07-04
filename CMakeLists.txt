cmake_minimum_required(VERSION 3.10)

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

if(PI_TARGET)
    # Include build functions from Pico SDK before declaring our project
    include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
endif()

project(spc C CXX ASM)

# add_library(spclib ${COMMON_SOURCES})

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)

FetchContent_Declare(
  cjson
  GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
  GIT_TAG        v1.7.15
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

# Fetch FTXUI
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v4.0.0
)

FetchContent_GetProperties(cjson)
if(NOT cjson_POPULATED)
  # include cjson, but don't build it - we will manually compile the files we want
  FetchContent_Populate(cjson)
endif()

# Add your common source files for both targets
file(GLOB_RECURSE COMMON_SOURCES
    "${CMAKE_SOURCE_DIR}/lib/*.c"
    "${CMAKE_SOURCE_DIR}/lib/*.cpp"
    "${cjson_SOURCE_DIR}/cJSON.c"
    "${cjson_SOURCE_DIR}/cJSON_Utils.c"
)

# Set compiler flags and options for each target
if(LINUX_TARGET)
    FetchContent_MakeAvailable(googletest)

    FetchContent_GetProperties(ftxui)
    if(NOT ftxui_POPULATED)
      FetchContent_Populate(ftxui)
      add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()

    # Linux target
    add_compile_definitions(LINUX)
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    set(CMAKE_CXX_STANDARD 17)

    # Add external libraries for Linux
    # find_package(Curses REQUIRED)

    include_directories("${CMAKE_SOURCE_DIR}")
    include_directories(${cjson_SOURCE_DIR})

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

    target_link_libraries(${PROJECT_NAME}
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component # Not needed for this example.
    )


    ## Testing
    # Add Google Test
    include_directories(googletest)

    file(GLOB_RECURSE TEST_SOURCES
        "${CMAKE_SOURCE_DIR}/test/*.c"
        "${CMAKE_SOURCE_DIR}/test/*.cpp"
    )

    # Add the test executable
    add_executable(spctest ${COMMON_SOURCES} ${TEST_SOURCES})

    # Link against the Google Test library
    target_link_libraries(spctest gtest_main)

    # Run the tests
    enable_testing()
    add_test(NAME spctest COMMAND spctest)
endif()

if(PI_TARGET)
    # Raspberry Pi Pico target
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 17)

    if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
        message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
    endif()

    # Initialize the SDK
    pico_sdk_init()

    include_directories("${CMAKE_SOURCE_DIR}")
    include_directories(${cjson_SOURCE_DIR})

    # Add your Pico source files
    file(GLOB_RECURSE PICO_SOURCES
        "${CMAKE_SOURCE_DIR}/src/main.cpp"
    )
    add_executable(${PROJECT_NAME} ${COMMON_SOURCES} ${PICO_SOURCES})

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
