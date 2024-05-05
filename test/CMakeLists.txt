SET(PROJECT_TEST ${PROJECT_NAME}-test )

include_directories(${CMAKE_SOURCE_DIR})

set(${PROJECT_TEST}_SOURCES
    OrderCacheTest.cpp
    ../OrderCacheImpl.h
    ../OrderCacheImpl.cpp
    ../OrderCacheInterface.h
    )

add_executable(${PROJECT_TEST} ${${PROJECT_TEST}_SOURCES})
target_link_libraries(${PROJECT_TEST} PRIVATE Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME}-test)
