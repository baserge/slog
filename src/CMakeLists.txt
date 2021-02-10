# CMakeLists.txt - cmake file, part of Slog.
# Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
# Licensed under GPLv3 or later, see the COPYING file.

set(SRC Logger.cpp
        StreamBuffer.cpp
        FileSink.cpp
        CoutThreadSink.cpp
        CerrThreadSink.cpp
        StringThreadSink.cpp
        FileThreadSink.cpp
        SharedFileThreadSink.cpp
        SharedStringThreadSink.cpp)


add_library(slog ${LIB_TYPE} ${SRC})
set(LIBS  )
target_link_libraries(slog ${LIBS})
install(
        TARGETS slog
        DESTINATION lib
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE
                    WORLD_READ WORLD_EXECUTE
       )
