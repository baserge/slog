# - Find Klubok (a simplistic multithreading)
# This module defines
# Klubok_INCLUDE_DIR, where to find Klubok headers
# Klubok_LIB, Klubok libraries
# Klubok_FOUND, If false, do not try to use Klubok

# Consider as Idontcare licence.

find_path(Klubok_INCLUDE_DIR Klubok/thread.h PATHS
            /usr/local/include
            /opt/local/include)
find_library(Klubok_LIB NAMES klubok PATHS
            /usr/local/lib
            /opt/local/lib)

if (Klubok_LIB AND Klubok_INCLUDE_DIR)
    set(Klubok_FOUND TRUE)
    set(Klubok_LIBS ${Klubok_LIB})
    message(STATUS "found Klubok: ${Klubok_LIB}")
else ()
    set(Klubok_FOUND FALSE)
    if (Klubok_FIND_REQUIRED)
        message(FATAL_ERROR "Klubok NOT found")
    else ()
        message(STATUS "Klubok not found")
    endif ()
endif ()

mark_as_advanced(
    Klubok_LIB
    Klubok_INCLUDE_DIR
  )
