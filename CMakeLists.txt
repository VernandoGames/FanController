# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(FanController C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(FanController 
	# Library executables
	include/mcp2515/mcp2515.cpp

	# Main executalbes
	# src/pico-mcp2515.cpp
	FanController.cpp
)

# Add MCP2515 Lib
target_include_directories(FanController PUBLIC include/)

pico_set_program_name(FanController "FanController")
pico_set_program_version(FanController "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(FanController 0)
pico_enable_stdio_usb(FanController 0)

# Add the standard library to the build
target_link_libraries(FanController
        pico_stdlib
		hardware_adc
		hardware_spi)

# Add the standard include files to the build
target_include_directories(FanController PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_enable_stdio_usb(FanController 1)
pico_enable_stdio_uart(FanController 0)

pico_add_extra_outputs(FanController)