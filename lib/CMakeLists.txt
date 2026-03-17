# Copyright 2011,2012,2016,2018,2019 Free Software Foundation, Inc.
# SPDX-License-Identifier: GPL-3.0-or-later

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

# --- NEW: Find HackRF Library ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(HACKRF REQUIRED libhackrf)
# --------------------------------

list(APPEND hackrf_sync_sources
    sync_source_impl.cc)

set(hackrf_sync_sources
    "${hackrf_sync_sources}"
    PARENT_SCOPE)

if(NOT hackrf_sync_sources)
    message(STATUS "No C++ sources... skipping lib/")
    return()
endif(NOT hackrf_sync_sources)

add_library(gnuradio-hackrf_sync SHARED ${hackrf_sync_sources})

# --- UPDATED: Link HackRF ---
target_link_libraries(gnuradio-hackrf_sync 
    gnuradio::gnuradio-runtime
    ${HACKRF_LIBRARIES}
)
# ----------------------------

target_include_directories(
    gnuradio-hackrf_sync
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    PUBLIC $<INSTALL_INTERFACE:include>
    ${HACKRF_INCLUDE_DIRS} # Add HackRF headers to path
)

set_target_properties(gnuradio-hackrf_sync PROPERTIES DEFINE_SYMBOL "gnuradio_hackrf_sync_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-hackrf_sync PROPERTIES INSTALL_NAME_DIR
                                                    "${CMAKE_INSTALL_PREFIX}/lib")
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
# Note: if gr_library_foo doesn't work on your version, 
# standard install commands apply, but modtool usually expects this:
gr_library_foo(gnuradio-hackrf_sync)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

list(APPEND test_hackrf_sync_sources)
list(APPEND GR_TEST_TARGET_DEPS gnuradio-hackrf_sync)

if(NOT test_hackrf_sync_sources)
    message(STATUS "No C++ unit tests... skipping")
    return()
endif(NOT test_hackrf_sync_sources)

foreach(qa_file ${test_hackrf_sync_sources})
    gr_add_cpp_test("hackrf_sync_${qa_file}" ${CMAKE_CURRENT_SOURCE_DIR}/${qa_file})
endforeach(qa_file)