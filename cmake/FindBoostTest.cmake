# - Find BoostTest (unit testing framework)
# This module defines
# BoostTest_INCLUDE_DIR, where to find BoostTest headers
# BoostTest_LIB, BoostTest libraries
# BoostTest_FOUND, If false, do not try to use BoostTest

# Consider as Idontcare licence.
find_path(BoostTest_INCLUDE_DIR boost/test/unit_test.hpp PATHS
            /usr/local/include
            /opt/local/include)
find_library(BoostTest_LIB NAMES boost_unit_test_framework PATHS
            /usr/local/lib
            /opt/local/lib)

if (BoostTest_LIB AND BoostTest_INCLUDE_DIR)
    set(BoostTest_FOUND TRUE)
    set(BoostTest_LIBS ${BoostTest_LIB})
    message(STATUS "found BoostTest: ${BoostTest_LIB}")
else ()
    set(BoostTest_FOUND FALSE)
    if (BoostTest_FIND_REQUIRED)
        message(FATAL_ERROR "BoostTest NOT found")
    else ()
        message(STATUS "BoostTest not found")
    endif ()
endif ()

mark_as_advanced(
    BoostTest_LIB
    BoostTest_INCLUDE_DIR
  )
