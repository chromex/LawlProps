cmake_minimum_required (VERSION 2.6)

project (LawlProps)

set (${PROJECT_NAME}_SRC
	"source/LawlProps.cpp"
	"test/Component.cpp"
	"test/GameObject.cpp"
	"test/Main.cpp"
)

set (${PROJECT_NAME}_INC
	"source/LawlProps.h"
	"test/Component.h"
	"test/GameObject.h"
	"test/Vec3.h"
)

add_library(LawlJSON STATIC IMPORTED)
set_property(TARGET LawlJSON PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../LawlJSON/libLawlJSON.a)

include_directories(${CMAKE_SOURCE_DIR}/../LawlJSON/Source)

source_group ("Source Files" FILES ${${PROJECT_NAME}_SRC})
source_group ("Header Files" FILES ${${PROJECT_NAME}_INC})

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/build)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/build)

add_definitions(-O2)

add_executable (${PROJECT_NAME}_test ${${PROJECT_NAME}_SRC} ${${PROJECT_NAME}_INC})
add_library (${PROJECT_NAME} ${${PROJECT_NAME}_SRC} ${${PROJECT_NAME}_INC})

target_link_libraries (${PROJECT_NAME} LawlJSON)
target_link_libraries (${PROJECT_NAME}_test LawlJSON)
