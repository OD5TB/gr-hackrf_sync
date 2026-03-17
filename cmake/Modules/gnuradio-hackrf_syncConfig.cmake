find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_HACKRF_SYNC gnuradio-hackrf_sync)

FIND_PATH(
    GR_HACKRF_SYNC_INCLUDE_DIRS
    NAMES gnuradio/hackrf_sync/api.h
    HINTS $ENV{HACKRF_SYNC_DIR}/include
        ${PC_HACKRF_SYNC_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_HACKRF_SYNC_LIBRARIES
    NAMES gnuradio-hackrf_sync
    HINTS $ENV{HACKRF_SYNC_DIR}/lib
        ${PC_HACKRF_SYNC_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-hackrf_syncTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_HACKRF_SYNC DEFAULT_MSG GR_HACKRF_SYNC_LIBRARIES GR_HACKRF_SYNC_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_HACKRF_SYNC_LIBRARIES GR_HACKRF_SYNC_INCLUDE_DIRS)
