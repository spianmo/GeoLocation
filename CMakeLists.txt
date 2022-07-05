cmake_minimum_required(VERSION 3.22)
project(GeoLocation)

set(CMAKE_CXX_STANDARD 17)

SET(GEOLOCATION_SRC main.cpp GeoCoordinateWatcher.cpp GeoCoordinateWatcher.h LocationCallback.cpp LocationCallback.h GeoStruct.h json.hpp)

ADD_LIBRARY(GeoLocation_static STATIC ${GEOLOCATION_SRC})
add_executable(GeoLocation ${GEOLOCATION_SRC})
