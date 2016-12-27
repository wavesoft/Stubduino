cmake_minimum_required (VERSION 2.8)

# Stubduino Project
project ( stubduino )
set(VERSION "0.0.1")

# Overridable C++11 parameters
set(STUBDUINO_SKETCH "" CACHE STRING "Path to your arduino .ino file" )
set(STUBDUINO_LIBS "" CACHE STRING "Path to your arduino libraries folder" )
set(STUBDUINO_HARDWARE "default" CACHE STRING "Hardware version to use" )

# Validate source
if (STUBDUINO_SKETCH STREQUAL "")
    message( FATAL_ERROR "Please specify the path to your sketch using the STUBDUINO_SKETCH= variable" )
endif()

#############################################################
# LIBRARIES
#############################################################

# For every library, we give the option to the project that
# includes us to provide them by itself.
#
# If not, we will try our best to build them as our child
# libraries (statically).
#
# To define a library, you must define the following two directives:
#   XXXXXX_LIBRARIES : Which contain the name(s) of the libraries
# XXXXXX_INCLUDE   : Which contain the additiona include directories
#

#
# [LUA] For lightweight scripting
#
set( LUA_MODE "EXTERN" )
if ( NOT DEFINED(LUA_LIBRARIES) OR NOT DEFINED(LUA_INCLUDE_DIRS) )

  # We are shipping mongoose with the project
  set( EXTERN_LUA "extern/lua" )
  add_subdirectory( ${EXTERN_LUA} ${CMAKE_BINARY_DIR}/${EXTERN_LUA} )

  # Log
  set( LUA_MODE "LOCAL" )
  message( STATUS "Using LUA shipped with project")

else()
  message( STATUS "Using LUA from: ${LUA_INCLUDE_DIRS}")

endif()

# Include libraries
set( PROJECT_INCLUDES
  ${LUA_INCLUDE_DIRS}
)

# Collect library names
set( PROJECT_LIBRARIES
  ${LUA_LIBRARIES}
)

# Add additional libraries on linux
if (UNIX AND NOT APPLE)
  set( PROJECT_LIBRARIES ${PROJECT_LIBRARIES} rt)
endif()

#############################################################
# ARDUINO HARDWARE
#############################################################

set (HARDWARE_DIR ${PROJECT_SOURCE_DIR}/src/hardware/${STUBDUINO_HARDWARE})
set (BOOTSTRAP_FILE ${HARDWARE_DIR}/bootstrap.cpp)

include_directories( ${HARDWARE_DIR}/include )

#############################################################
# ARDUINO BOOTSTRAP
#############################################################

set (BOOTSTRAP_SOURCE ${CMAKE_BINARY_DIR}/bootstrap.cpp)

add_custom_command(OUTPUT ${BOOTSTRAP_SOURCE}
  MAIN_DEPENDENCY
    ${STUBDUINO_SKETCH}
  COMMAND
    ${CMAKE_COMMAND}
      -DBOOTSTRAP_FILE="${BOOTSTRAP_FILE}"
      -DBOOTSTRAP_SOURCE="${BOOTSTRAP_SOURCE}"
      -DSTUBDUINO_SKETCH="${STUBDUINO_SKETCH}"
      -P "${PROJECT_SOURCE_DIR}/cmake/CreateBootstrap.cmake"
)

#############################################################
# ARDUINO LIBRARIES
#############################################################

set ( ARDUINO_LIB_INCLUDES "" )
set ( ARDUINO_LIB_SOURCES "" )

if (NOT STUBDUINO_LIBS STREQUAL "")
  file ( GLOB ARDUINO_LIB_DIRS ${STUBDUINO_LIBS}/* )
  file ( READ ${STUBDUINO_SKETCH} SKETCH_CONTENTS )
  string (REGEX MATCHALL "#include[\t ]+<([^>]+)>" MATCHED_IMPORTS ${SKETCH_CONTENTS})

  # For every import statement, check on which library is it found
  foreach(IMPORT ${MATCHED_IMPORTS})
    string(REGEX REPLACE "#include[\t ]+<|>" "" IMPORT ${IMPORT})

    foreach(LIB_DIR ${ARDUINO_LIB_DIRS})
      if (EXISTS "${LIB_DIR}/${IMPORT}")
        message(STATUS "Using arduino library " ${LIB_DIR})
        file (GLOB_RECURSE LIB_CFILES
          ${LIB_DIR}/*.c
          ${LIB_DIR}/*.cpp
        )
        list (APPEND ARDUINO_LIB_INCLUDES ${LIB_DIR})
        list (APPEND ARDUINO_LIB_SOURCES ${LIB_CFILES})
      endif()
    endforeach(LIB_DIR)
  endforeach(IMPORT)
endif()

#############################################################
# SOURCES
#############################################################

# Locate the daemon sources
file ( GLOB STUBDUINO_SOURCES
    ${ARDUINO_LIB_SOURCES}
    ${HARDWARE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/emulated/*.cpp
  )

# Setup includes
include_directories( ${ARDUINO_LIB_INCLUDES} )
include_directories( ${PROJECT_SOURCE_DIR}/src )
include_directories( ${PROJECT_INCLUDES} )

#############################################################
# TARGET & LINK
#############################################################

# Sources
add_executable( ${PROJECT_NAME}
  ${STUBDUINO_SOURCES}
  ${BOOTSTRAP_SOURCE}
  )

# Libraries
target_link_libraries ( ${PROJECT_NAME} ${PROJECT_LIBRARIES} )
