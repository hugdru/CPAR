cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(project_name ii-openmp-improved-sieve_of_erastosthenes)
project(${project_name} CXX)

set(src_dir src)
file(GLOB_RECURSE src_files ${src_dir}/*.cpp ${src_dir}/*.hpp)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default Release)" FORCE)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_DEBUG "-pipe -Wundef -Wstrict-overflow=5 -Wsign-promo -Woverloaded-virtual -Wold-style-cast -Wctor-dtor-privacy -Wformat=2 -Winvalid-pch -Wmissing-include-dirs -Wpacked -Wpadded -Wall -Wextra -pedantic -Wdouble-promotion -Wshadow -Wfloat-equal -Wcast-align -Wcast-qual -Wwrite-strings -Wconversion -Wsign-conversion -Wmissing-declarations -Wredundant-decls -Wdisabled-optimization -Winline -Wswitch-default -Wswitch-enum")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -pipe -s -DNDEBUG -D_FORTIFY_SOURCE=1 -fstack-protector-strong -Wdisabled-optimization -Wstack-protector -Winline")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS_DEBUG "-ggdb ${CMAKE_CXX_FLAGS_DEBUG} -Wuseless-cast -Wlogical-op -Wzero-as-null-pointer-constant -Wnoexcept -Wstrict-null-sentinel")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS_DEBUG "-g ${CMAKE_CXX_FLAGS_DEBUG}")
    endif()
endif()

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

if(OPENMP_FOUND)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS}")
else()
  message(FATAL_ERROR "A compiler with OpenMP support is required")
endif()

add_executable(${project_name} ${src_files})
target_link_libraries(${project_name} Threads::Threads)
