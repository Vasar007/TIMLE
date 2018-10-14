cmake_minimum_required(VERSION 3.8)

# Enable debug symbols by default, must be done before project() statement.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

project(TIMLE)

# Set SFML path in the system.
if (WIN32)
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

# Set additional properties for x64 platform.
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS FALSE)
else ()
    message(FATAL_ERROR "Unsupported architecture.")
endif()

# Add TinyXML library.
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

# Not used static linking but it need to think abut this option.
set(SFML_STATIC_LIBS FALSE CACHE BOOL "Choose whether SFML is linked statically or not.")

# Detect and add SFML
find_package(SFML 2.5 COMPONENTS audio graphics window system REQUIRED)

# Check if we found SFML.
if (NOT SFML_FOUND)
    message(FATAL_ERROR "Not found SFML library.")
endif ()

# Include sub-projects.
add_subdirectory(TIMLE)
