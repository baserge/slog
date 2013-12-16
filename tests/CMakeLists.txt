# CMakeLists.txt - cmake file, part of Slog.
# Copyright 2013 Sergey Bozhenkov, boz at ipp.mpg.de
# Licensed under GPLv3 or later, see the COPYING file.
set(TEST_SRC  main.cpp
              logger.cpp)
set(LIBS slog
         boost_unit_test_framework)
add_executable(tests EXCLUDE_FROM_ALL ${TEST_SRC})
target_link_libraries(tests ${LIBS})
