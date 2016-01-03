cmake_minimum_required(VERSION 2.6)
project(timetest)

ADD_LIBRARY (bcm2835 STATIC IMPORTED)
SET_PROPERTY(TARGET bcm2835 PROPERTY IMPORTED_LOCATION "/home/pi/bcm2835-1.48/src/libbcm2835.a")

include_directories("/home/pi/bcm2835-1.48/src/")

add_executable(timetest main.cpp)
target_link_libraries(timetest bcm2835)

install(TARGETS timetest RUNTIME DESTINATION bin)

