# Copyright 2011-2020 Free Software Foundation, Inc.
# SPDX-License-Identifier: GPL-3.0-or-later

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "")

########################################################################
# Project setup
########################################################################
cmake_minimum_required(VERSION 3.16)
project(gr-hackrf_sync CXX C)
enable_testing()

if(DEFINED ENV{PYBOMBS_PREFIX})
    set(CMAKE_INSTALL_PREFIX $ENV{PYBOMBS_PREFIX} CACHE PATH "")
    message(STATUS "PyBOMBS installed GNU Radio. Defaulting CMAKE_INSTALL_PREFIX to $ENV{PYBOMBS_PREFIX}")
endif()

list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(Gnuradio "3.10" REQUIRED)

# --- NEW: Find HackRF Dependency Globally ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(HACKRF REQUIRED libhackrf)
if(HACKRF_FOUND)
    message(STATUS "Found HackRF: ${HACKRF_LIBRARIES}")
else()
    message(FATAL_ERROR "HackRF library (libhackrf) not found! Please install libhackrf-dev.")
endif()
# --------------------------------------------

set(VERSION_MAJOR 1)
set(VERSION_API   0)
set(VERSION_ABI   0)
set(VERSION_PATCH 0)

cmake_policy(SET CMP0011 NEW)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GrMinReq)
include(GrCompilerSettings)
include(GrVersion)
include(GrPlatform) 

if(NOT CMAKE_MODULES_DIR)
    set(CMAKE_MODULES_DIR lib${LIB_SUFFIX}/cmake)
endif(NOT CMAKE_MODULES_DIR)

set(GR_INCLUDE_DIR include/gnuradio/hackrf_sync)
set(GR_CMAKE_DIR ${CMAKE_MODULES_DIR}/gnuradio-hackrf_sync)
set(GR_PKG_DATA_DIR ${GR_DATA_DIR}/${CMAKE_PROJECT_NAME})
set(GR_PKG_DOC_DIR ${GR_DOC_DIR}/${CMAKE_PROJECT_NAME})
set(GR_PKG_CONF_DIR ${GR_CONF_DIR}/${CMAKE_PROJECT_NAME}/conf.d)
set(GR_PKG_LIBEXEC_DIR ${GR_LIBEXEC_DIR}/${CMAKE_PROJECT_NAME})

if(APPLE)
    if(NOT CMAKE_INSTALL_NAME_DIR)
        set(CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/${GR_LIBRARY_DIR} CACHE PATH "" FORCE)
    endif()
    if(NOT CMAKE_INSTALL_RPATH)
        set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${GR_LIBRARY_DIR} CACHE PATH "" FORCE)
    endif()
    set(CMAKE_BUILD_WITH_INSTALL_RPATH ON CACHE BOOL "" FORCE)
endif(APPLE)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    option(ENABLE_DOXYGEN "Build docs using Doxygen" ON)
else(DOXYGEN_FOUND)
    option(ENABLE_DOXYGEN "Build docs using Doxygen" OFF)
endif(DOXYGEN_FOUND)

configure_file(${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake @ONLY)

add_custom_target(uninstall ${CMAKE_COMMAND} -P
                            ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

########################################################################
# Add subdirectories
########################################################################
add_subdirectory(include/gnuradio/hackrf_sync)
add_subdirectory(lib)
add_subdirectory(apps)
add_subdirectory(examples)
add_subdirectory(docs)

if(ENABLE_PYTHON)
    message(STATUS "PYTHON and GRC components are enabled")
    add_subdirectory(python/hackrf_sync)
    add_subdirectory(grc)
else(ENABLE_PYTHON)
    message(STATUS "PYTHON and GRC components are disabled")
endif(ENABLE_PYTHON)

install(FILES cmake/Modules/gnuradio-hackrf_syncConfig.cmake DESTINATION ${GR_CMAKE_DIR})

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/Modules/targetConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_Modules/hackrf_syncConfig.cmake
    INSTALL_DESTINATION ${GR_CMAKE_DIR})

install(FILES MANIFEST.yml
    RENAME MANIFEST-${VERSION_MAJOR}.${VERSION_API}.${VERSION_ABI}${VERSION_PATCH}.yml
    DESTINATION share/gnuradio/manifests/hackrf_sync)