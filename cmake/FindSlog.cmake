# - Find Slog (C++ logging)
# This module defines
# Slog_INCLUDE_DIR, where to find Slog headers
# Slog_LIB, Slog libraries
# Slog_FOUND, If false, do not try to use Slog

# Consider as Idontcare licence.

find_path(Slog_INCLUDE_DIR Slog/Logger.h PATHS
            /usr/local/include
            /opt/local/include)
find_library(Slog_LIB NAMES slog PATHS
            /usr/local/lib
            /opt/local/lib)

if (Slog_LIB AND Slog_INCLUDE_DIR)
    set(Slog_FOUND TRUE)
    set(Slog_LIBS ${Slog_LIB})
    message(STATUS "found Slog: ${Slog_LIB}")
else ()
    set(Slog_FOUND FALSE)
    if (Slog_FIND_REQUIRED)
        message(FATAL_ERROR "Slog NOT found")
    else ()
        message(STATUS "Slog not found")
    endif ()
endif ()

mark_as_advanced(
    Slog_LIB
    Slog_INCLUDE_DIR
  )
