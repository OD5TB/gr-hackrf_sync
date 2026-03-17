# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT hackrf_sync_sources)
    message(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT hackrf_sync_sources)

########################################################################
# Check for pygccxml
########################################################################
gr_python_check_module_raw("pygccxml" "import pygccxml" PYGCCXML_FOUND)

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND hackrf_sync_python_files
    sync_source_python.cc python_bindings.cc)

gr_pybind_make_oot(hackrf_sync ../../.. gr::hackrf_sync "${hackrf_sync_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(
    TARGET hackrf_sync_python
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hackrf_sync_python>
            ${PROJECT_BINARY_DIR}/test_modules/gnuradio/hackrf_sync/)

install(
    TARGETS hackrf_sync_python
    DESTINATION ${GR_PYTHON_DIR}/gnuradio/hackrf_sync
    COMPONENT pythonapi)
