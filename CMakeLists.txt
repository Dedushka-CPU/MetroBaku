cmake_minimum_required(VERSION 2.8)

project(MetroSimulator)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_library(MetroLib
    Station.cpp
    Train.cpp
)

target_include_directories(MetroLib PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(metro
    main.cpp
    Station.cpp
    Train.cpp
)

target_link_libraries(metro PRIVATE MetroLib)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
target_link_libraries(metro PRIVATE Threads::Threads)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
