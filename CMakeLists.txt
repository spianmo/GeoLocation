cmake_minimum_required(VERSION 3.22)
project(GeoLocation)

set(CMAKE_CXX_STANDARD 17)

# some atl settings
SET(CMAKE_CXX_WARNING_LEVEL "4" CACHE STRING "Warning level" FORCE)
SET(CMAKE_CXX_USE_RTTI YES CACHE BOOL "RTTI" FORCE)
SET(CMAKE_CXX_WIN9X_UNICODE NO CACHE BOOL "WIN9X_UNICODE" FORCE)
SET(CMAKE_ATL_FLAG "1" CACHE STRING "ATL" FORCE)
ADD_DEFINITIONS(-D_ATL_ATTRIBUTES -D_USRDLL)
SET(ATL_LIBS comsuppw)

# Compiler options
ADD_DEFINITIONS(-D_ATL_ATTRIBUTES -D_WINDLL -D_USRDLL)

SET(OS_LIBS ${ATL_LIBS})

add_executable(GeoLocation main.cpp GeoCoordinateWatcher.cpp GeoCoordinateWatcher.h LocationCallback.cpp LocationCallback.h GeoStruct.h json.hpp)
