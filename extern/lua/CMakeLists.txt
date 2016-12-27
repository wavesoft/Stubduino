cmake_minimum_required(VERSION 2.4.4)
project(lua C)

# Version
set(LUA_VERSION 5.3.2)

#############################################################
# DOWNLOAD SCRIPT
#############################################################

# Setup archive
set( ARCHIVE_URL "https://www.lua.org/ftp/lua-${LUA_VERSION}.tar.gz" )
set( ARCHIVE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lua-${LUA_VERSION}" )
set( ARCHIVE_EXT ".tar.gz" )

# Check if we have the archive
if ( NOT EXISTS ${ARCHIVE_DIR} )
    set( ARCHIVE_FILE "${ARCHIVE_DIR}${ARCHIVE_EXT}" )

    # Make the directory
    FILE( MAKE_DIRECTORY ${ARCHIVE_DIR} )

    # Download the archive
    message( STATUS "Downloading ${PROJECT_NAME} archive" )
    file(DOWNLOAD ${ARCHIVE_URL} ${ARCHIVE_FILE}
        SHOW_PROGRESS STATUS ARCHIVE_DOWNLOAD_STATUS)

    # Check status
    list( GET ARCHIVE_DOWNLOAD_STATUS 0 ARCHIVE_DOWLOAD_ERROR )
    if( ARCHIVE_DOWLOAD_ERROR )

        # Remove file and dir
        file( REMOVE ${ARCHIVE_FILE} )
        file( REMOVE_RECURSE ${ARCHIVE_DIR} )

        # Fatal error
        message( FATAL_ERROR "Could not download ${PROJECT_NAME} archive!" )
    endif()

    # Extract archive
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${ARCHIVE_FILE}
        WORKING_DIRECTORY ${ARCHIVE_DIR}
    )

endif()

# Get the first sub-directory in the archive
FILE(GLOB _ARCHIVE_SUBDIRS ${ARCHIVE_DIR}/*)
FOREACH( _ARCHIVE_SUBDIR ${_ARCHIVE_SUBDIRS} )
    IF(IS_DIRECTORY ${_ARCHIVE_SUBDIR})
        SET(ARCHIVE_DIR ${_ARCHIVE_SUBDIR})
    ENDIF()
ENDFOREACH()

# Rebase
set( CMAKE_CURRENT_SOURCE_DIR ${ARCHIVE_DIR} )

# Expose project variables to parent
set (LUA_LIBRARIES ${PROJECT_NAME} m PARENT_SCOPE)
set (LUA_INCLUDE_DIRS ${ARCHIVE_DIR}/src PARENT_SCOPE)

#############################################################
# BUILD SCRIPT 
#############################################################

# Include directories
include_directories (
    ${ARCHIVE_DIR}/src
)

# Set sources
set(LUA_SRC

    # Core
    ${ARCHIVE_DIR}/src/lapi.c
    ${ARCHIVE_DIR}/src/lcode.c
    ${ARCHIVE_DIR}/src/lctype.c
    ${ARCHIVE_DIR}/src/ldebug.c
    ${ARCHIVE_DIR}/src/ldo.c
    ${ARCHIVE_DIR}/src/ldump.c
    ${ARCHIVE_DIR}/src/lfunc.c
    ${ARCHIVE_DIR}/src/lgc.c
    ${ARCHIVE_DIR}/src/llex.c
    ${ARCHIVE_DIR}/src/lmem.c
    ${ARCHIVE_DIR}/src/lobject.c
    ${ARCHIVE_DIR}/src/lopcodes.c
    ${ARCHIVE_DIR}/src/lparser.c
    ${ARCHIVE_DIR}/src/lstate.c
    ${ARCHIVE_DIR}/src/lstring.c
    ${ARCHIVE_DIR}/src/ltable.c
    ${ARCHIVE_DIR}/src/ltm.c
    ${ARCHIVE_DIR}/src/lundump.c
    ${ARCHIVE_DIR}/src/lvm.c
    ${ARCHIVE_DIR}/src/lzio.c

    # Library
    ${ARCHIVE_DIR}/src/lauxlib.c
    ${ARCHIVE_DIR}/src/lbaselib.c
    ${ARCHIVE_DIR}/src/lbitlib.c
    ${ARCHIVE_DIR}/src/lcorolib.c
    ${ARCHIVE_DIR}/src/ldblib.c
    ${ARCHIVE_DIR}/src/liolib.c
    ${ARCHIVE_DIR}/src/lmathlib.c
    ${ARCHIVE_DIR}/src/loslib.c
    ${ARCHIVE_DIR}/src/lstrlib.c
    ${ARCHIVE_DIR}/src/ltablib.c
    ${ARCHIVE_DIR}/src/lutf8lib.c
    ${ARCHIVE_DIR}/src/loadlib.c
    ${ARCHIVE_DIR}/src/linit.c

)

# Link library
add_library (${PROJECT_NAME} STATIC ${LUA_SRC})

