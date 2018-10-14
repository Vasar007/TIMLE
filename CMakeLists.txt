cmake_minimum_required(VERSION 3.8)

# Enable debug symbols by default
# must be done before project() statement
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

project(TIMLE)

if(WIN32)
    set(CMAKE_PREFIX_PATH "C:/Program Files (x86)/Common Files/SFML-2.5.0-x64")
endif()

# Set compiler options.
if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler build variables.
set(TARGET_CPU amd64)
set(BUILD_CONFIGURATION Debug)

if ("${TARGET_CPU}" STREQUAL "amd64")
    set(CMAKE_SIZEOF_VOID_P 8)

    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS FALSE)
elseif ("${TARGET_CPU}" STREQUAL "x86")
    set(CMAKE_SIZEOF_VOID_P 4)

    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS TRUE)

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
        set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m32")
    endif()
else ()
    message (FATAL_ERROR "Unsupported CPU: ${TARGET_CPU}")
endif()

# Add cx_options library.
add_library(TinyXML STATIC
    Dependencies/TinyXML/tinystr.h
    Dependencies/TinyXML/tinystr.cpp
    Dependencies/TinyXML/tinyxml.h
    Dependencies/TinyXML/tinyxml.cpp
    Dependencies/TinyXML/tinyxmlerror.cpp
    Dependencies/TinyXML/tinyxmlparser.cpp
)
target_include_directories(TinyXML PUBLIC
    Dependencies
)
set_target_properties(TinyXML PROPERTIES LINKER_LANGUAGE CXX)

# Include sub-projects.
add_subdirectory(TIMLE)
