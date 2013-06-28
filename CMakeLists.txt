cmake_minimum_required(VERSION 2.8)

project( beeper )

include_directories(${BEEPER_SOURCE_DIR} ".")
add_definitions(-Wall -std=c++11 -g3)

set(CMAKE_CXX_FLAGS "-framework OpenAL")

add_executable( beeper beeper.cxx )

