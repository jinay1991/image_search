cmake_minimum_required(VERSION 2.8)

project(image_search)

find_package(OpenCV REQUIRED)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ColorDescriptor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/search.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Searcher.cpp)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIR}
)

include_directories(${HEADERS})

add_executable(search ${SOURCES})
target_link_libraries(search ${OpenCV_LIBS})

add_executable(index ${CMAKE_CURRENT_SOURCE_DIR}/src/index.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/ColorDescriptor.cpp)
target_link_libraries(index ${OpenCV_LIBS})