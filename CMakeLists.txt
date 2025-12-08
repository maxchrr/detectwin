cmake_minimum_required(VERSION 3.15) # FetchContent is available in 3.11+
project(detectwin)
set(CMAKE_C_STANDARD 99) # Requires C99 standard
set(MY_C_FLAGS ${CMAKE_C_FLAGS} "-O0" "-Wall" "-Wextra" "-pedantic" "-Wno-"format-truncation" "-Wno-unused-but-set-variable")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adding our source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/sources/*.c") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/includes/") # Define PROJECT_INCLUDE to be the path to the include directory of the project

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} ncurses)
target_compile_options(${PROJECT_NAME} PRIVATE ${MY_C_FLAGS})
