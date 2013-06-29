cmake_minimum_required(VERSION 2.8)

project( beeper )

include_directories(${BEEPER_SOURCE_DIR} ".")
if( APPLE )
    add_definitions(-Wall -std=c++11 -g3)
    set(CMAKE_CXX_FLAGS "-framework OpenAL")
else()
    add_definitions(-Wall -std=c++11 -g3)
    set(CMAKE_CXX_FLAGS "-l openal")

endif()
add_executable( beeper beeper.cxx )

