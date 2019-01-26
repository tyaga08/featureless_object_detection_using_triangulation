cmake_minimum_required(VERSION 3.5)
project(CV)

IF(NOT CMAKE_BUILD_TYPE)
SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(OpenCV REQUIRED)
#find_package(PythonLibs 2.7)

add_compile_options(-std=c++11)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -march=native ")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native -std=c++11 -Wno-reorder")

include_directories(dir
${PROJECT_SOURCE_DIR}/
${PROJECT_SOURCE_DIR}/include/
#${PROJECT_SOURCE_DIR}/../matplotlib-cpp/
)

add_executable(image_recognition src/image_recognition.cpp)
target_include_directories(image_recognition PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(image_recognition ${OpenCV_LIBS} ${PYTHON_LIBRARIES})

