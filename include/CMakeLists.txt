file(GLOB Slog_H "include/*.h")
set(Slog_H ${Slog_H}
          ${CMAKE_BINARY_DIR}/config.h)
install(FILES ${Slog_H}
        DESTINATION include/Slog
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        COMPONENT DEVELOPMENT
       )
