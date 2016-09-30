cmake_minimum_required(VERSION 2.6)
project (ExoSim)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -pthread")

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/" )

find_package(SFML REQUIRED)
find_package(SFGUI REQUIRED)
find_package(CURL REQUIRED)

include_directories(include)

file(GLOB ${PROJECT_NAME}_SRCS 	
	${PROJECT_SOURCE_DIR}/src/*.cpp
	${PROJECT_SOURCE_DIR}/include/*.h
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})

if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()

if(SFGUI_FOUND)
	include_directories(${SFGUI_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${SFGUI_LIBRARIES})
endif()

if(CURL_FOUND)
	include_directories(${CURL_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})
endif()

if(APPLE)
   include_directories(AFTER "/opt/X11/include")
endif()