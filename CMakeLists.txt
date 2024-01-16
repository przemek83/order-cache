cmake_minimum_required(VERSION 3.8)

project(tradeweb
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "Interview coding assignment."
    HOMEPAGE_URL "https://github.com/przemek83/tradeweb")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(${PROJECT_NAME}_SOURCES
    main.cpp
    OrderCache.h
    OrderCache.cpp
    OrderCacheInterface.h
    )

add_executable(${CMAKE_PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

enable_testing()
Include(Tests.cmake)
add_subdirectory(test)
