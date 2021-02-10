# Licensed under GPLv3 or later, see the COPYING file.

set(SRC Logger.cpp
        FileSink.cpp
        CoutThreadSink.cpp
        CerrThreadSink.cpp
        FileThreadSink.cpp
        SharedFileThreadSink.cpp
        Header.cpp)


add_library(slog ${LIB_TYPE} ${SRC})
set(LIBS  ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(slog ${LIBS})
install(
        TARGETS slog
        DESTINATION lib
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE
                    WORLD_READ WORLD_EXECUTE
       )
