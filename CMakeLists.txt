# Licensed under GPLv3 or later, see the COPYING file.
cmake_minimum_required(VERSION 3.0)
project(Slog VERSION 0.2.0)
# -- end project name, etc.

# -- settings
set(CMAKE_BUILD_TYPE Release) #defined DNDEBUG and O3
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INSTALL_PREFIX "/usr/local")
#set(DEFS -O2 -march=native
    #-mfpmath=sse -msse -msse2 -mssse3 -msse4.1
    #-fno-math-errno -ffinite-math-only -fno-signed-zeros
    #-fno-trapping-math -fno-signaling-nans
    #-fomit-frame-pointer -falign-labels=1 -falign-jumps=1
    #-funroll-loops  -finline-functions -Wall -Wextra
    #-Wconversion -Wpointer-arith -Wcast-qual
    #-g)#0 -s)
set(LIB_TYPE SHARED) #SHARED STATIC MODULE
# -- end settings

# find dependencies
set(CMAKE_MODULE_PATH ${CMAKE_ROOT}/Modules ${CMAKE_SOURCE_DIR}/cmake)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads)
find_package(BoostTest)
# -- end find dependencies

# -- do configure file
configure_file(./config.h.in ./config.h)
# -- end do configure file

# -- compiler flags
#if(CMAKE_COMPILER_IS_GNUCXX)
    #if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        #set(DEFS ${DEFS} -m64)
    #endif()
    #add_definitions(${DEFS})
    #set (CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS_INIT} ${DEFS})
    #set (CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS_INIT} ${DEFS})
#endif()
# -- end compiler flags

# -- add sub-projects
include_directories(${CMAKE_BINARY_DIR} ./include)
include(include/CMakeLists.txt)
add_subdirectory(./src lib)
if (BoostTest_FOUND)
    add_subdirectory(./tests)
else ()
    message(WARNING " BoostTest was not found, tests will not be built.")
endif ()
